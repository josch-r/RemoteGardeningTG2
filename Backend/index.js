const express = require("express");
const axios = require("axios");
const nodemailer = require('nodemailer');
let plantsCollection;
const app = express();
const port = 3000;

//db connection
const MongoClient = require("mongodb").MongoClient;
const { ObjectId } = require("bson");
const uri =
    "mongodb+srv://RemoteGardeningAdmin:LQu5bDDvZ2aziOSq@getraenkelistedb.4hnwb.mongodb.net/RemoteGardening?retryWrites=true&w=majority";
const client = new MongoClient(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
});

async function connectDB() {
    client.connect((err) => {
        plantsCollection = client.db("RemoteGardening").collection("plants");
        if (err) throw err;
    });
}
connectDB();

var address,
    ifaces = require('os').networkInterfaces();
for (var dev in ifaces) {
    ifaces[dev].filter((details) => details.family === 'IPv4' && details.internal === false ? address = details.address : undefined);
}

console.log(address);

async function sendNotiMail(uid, wifiName) {
    let transporter = nodemailer.createTransport({
        service: 'gmail',
        auth: {
            user: 'remote.gardining@gmail.com',
            pass: 'WS2020iot'
        }
    });

    let mailOptions = {
        from: 'remote.gardining@gmail.com',
        to: 'joschua.rothenbacher@hfg.design, jannes.blobel@hfg.design, fabienne.vatter@hfg.design',
        subject: 'A new Plant has been added to your remote garden!',
        text: 'Hi ihr Aperolis! Eine neue Pflanze wurde erkannt die UID dazu ist ' + uid + '. Der Wifi name lautet: ' + wifiName + 'GaLiGrü! Euer Backend <3',
    };

    transporter.sendMail(mailOptions, function (error, info) {
        if (error) {
            console.log(error);
        } else {
            console.log('Email sent: ' + info.response);
        }
    });
}

app.use(express.json());
app.get("/", (req, res) => {
    res.send("service is alive");
});

app.post("/plant/register", async (req, res) => {
    let rfidUID = req.body.uid;
    let wifiName = req.body.wifiName;
    console.log(rfidUID);
    //add new mongoDB entry
    plantsCollection.insertOne({
        rfidUID: rfidUID,
        name: "default",
        waterLevel: 3,
        lastWatered: "0/0/0",
        owner: "none",
        plantSize: "S",
    });
    res.status(200).send("sucessfully added plant").end();
    await sendNotiMail(rfidUID, wifiName);
});
function decideWater(entry) {
    let timeNow = Date.now() / 1000;
    let waterTime = 0;
    let lastWatered = entry.lastWatered / 1000;
    let timeDifference = timeNow - lastWatered;
    console.log("The Time difference is: " + timeDifference + " in seconds");
    switch (entry.waterLevel) {
        case 1:
            waterTime = 20000;
            updateLastWatered(entry.rfidUID);
            break;
        case 2:
            if (timeDifference > 43200) {
                waterTime = 20000;
            }
            break;
        case 3:
            if (timeDifference > 86400) {
                waterTime = 20000;
            }
            break;
        case 4:
            if (timeDifference > 172800) {
                waterTime = 20000;
            }
            break;
        case 5:
            if (timeDifference > 259200) {
                waterTime = 20000;
            }
            break;
    }
    if (waterTime == 0) {
        console.log("this plant doesn't need Water right now");
    }
    return waterTime;
}

app.get("/plant/status/:uid", async (req, res) => {
    let rfidUID = req.params.uid;
    console.log(rfidUID);
    //get mongoDB entry for UID
    plantsCollection.findOne({ 'rfidUID': rfidUID }, (err, result) => { //TODO nsyntax
        if (err) throw err;
        if (result !== null) {
            console.log(result);
            let waterMS = decideWater(result);
            res.status(200).send(waterMS.toString()).end();
        } else {
            res.status(404).send("plant doesnt exist in DB").end();
        }
    });
});
async function updateLastWatered(ID) {
    let newWateredDate = { $set: { lastWatered: Date.now() } };
    plantsCollection.updateOne({ 'rfidUID': ID }, newWateredDate, (err, res) => {
        if (err) throw err;
        else {
            console.log("sucessfully updated DB entry");
        }
    });
}
app.listen(port, () => {
    console.log(`RemoteGardeningBackend listening at http://localhost:${port}`);
});