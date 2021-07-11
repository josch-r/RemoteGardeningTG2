const express = require("express");
const axios = require("axios");
const nodemailer = require('nodemailer');
const cors = require("cors");
let plantsCollection;
const app = express();
const port = 3000;

//db connection
const MongoClient = require("mongodb").MongoClient;
const { ObjectId } = require("bson");
const { ppid } = require("process");
const uri =
    "mongodb+srv://RemoteGardeningAdmin:LQu5bDDvZ2aziOSq@getraenkelistedb.4hnwb.mongodb.net/RemoteGardening?retryWrites=true&w=majority";
const client = new MongoClient(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
});
//connect to our mongodb
async function connectDB() {
    client.connect((err) => {
        plantsCollection = client.db("RemoteGardening").collection("plants");
        getAllPlants();
        if (err) throw err;
    });
}
connectDB();

//Output out local ip adress to get the right connection to the esp
var address,
    ifaces = require('os').networkInterfaces();
for (var dev in ifaces) {
    ifaces[dev].filter((details) => details.family === 'IPv4' && details.internal === false ? address = details.address : undefined);
}
console.log(address);

//Function to send our notification email if a new plant has been scanned.
async function sendNotiMail(uid, wifiName) {
    let transporter = nodemailer.createTransport({
        //service: 'gmail',
        host: "smtp.gmail.com",
        port: 587,
        secure: false, // true for 465, false for other ports
        auth: {
            user: 'remote.gardining@gmail.com',
            pass: '#WS2020iot'
        }
    });

    let mailOptions = {
        from: 'remote.gardining@gmail.com',
        to: 'joschua.rothenbacher@hfg.design, jannes.blobel@hfg.design, fabienne.vatter@hfg.design',
        subject: 'A new Plant has been added to your remote garden!',
        text: 'Hi ihr Aperolis! Eine neue Pflanze wurde erkannt die UID dazu ist : ' + uid + '. Der Wifi name lautet: ' + wifiName +
         'Ihr könnt nun diese Pflanze unter der Adresse: http://'+ address + ':8080/add/'+uid+' bearbeiten! GaLiGrü! Euer Backend <3',
    };

    transporter.sendMail(mailOptions, function(error, info) {
        if (error) {
            console.log(error);
        } else {
            console.log('Email sent: ' + info.response);
        }
    });
}

app.use(cors());
app.use(express.json());
app.get("/", (req, res) => {
    res.send("service is alive");
});

//Post endpoint to register a new plant
app.post("/plant/register", async(req, res) => {
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

//Function to compare the current time and the last Watered time and send the time in ms to water or not to water
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
            if (timeDifference > 21600) {
                waterTime = 20000;
            }
            break;
        case 3:
            if (timeDifference > 43200) {
                waterTime = 20000;
            }
            break;
        case 4:
            if (timeDifference > 151200) {
                waterTime = 20000;
            }
            break;
        case 5:
            if (timeDifference > 201600) {
                waterTime = 20000;
            }
            break;
        case 6:
            if (timeDifference > 302400) {
                    waterTime = 20000;
                }
                break;
    }
    if (waterTime == 0) {
        console.log("this plant doesn't need Water right now");
    }
    return waterTime;
}

//GET endpoint to get the plant entry in our DB and to decide if it needs water in this moment
app.get("/plant/status/:uid", async(req, res) => {
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
//If the plant was watered update the database entry with the new value for lastWatered
async function updateLastWatered(ID) {
    let newWateredDate = { $set: { lastWatered: Date.now() } };
    plantsCollection.updateOne({ 'rfidUID': ID }, newWateredDate, (err, res) => {
        if (err) throw err;
        else {
            console.log("sucessfully updated DB entry");
        }
    });
}


/// Frontend API endpoints
//GET Endpoint for our frontend with the values of the chosen plant
app.get("/overview/:id", async(req, res) => {
    console.log(req.params.id);
    try {
        let code = req.params.id;
        let result = await plantsCollection.findOne({ "rfidUID": code });
        if (result == undefined) {
            res.status(404).end();
        } else {
            res.status(200).send({ name: result.name, description: result.description, waterlevel: result.waterlevel, lastWatered: result.lastWatered, ownerId: result.owner, Size: result.plantSize });
        }
    }
    catch(error) {
        console.log(error);
        res.status(404).send("No MongoDB entry found!");
    }
});
//Post endpoint to update the plant entry with new values
app.post("/overview/update", async(req, res) => {
    console.log(req.body);
    updatePlant(req.body);
});
//Function to get all current entries in our DB 
async function getAllPlants(){
   plantsCollection.find({}).toArray(function(err, result) {
        console.log("Your entries in MongoDB are: ");
        console.log(result);
    });
}

//Update plant entry with new values
async function updatePlant(plant) {
    console.log("plant is: " + plant);
    let addInfos = { $set: {name: plant.name, owner: plant.owner, plantsize : plant.growth, description: plant.description, waterLevel: plant.waterLevel }};
    plantsCollection.updateOne({ "rfidUID": plant.rfidUID }, addInfos, (err, res) => {
        if (err) throw err;
        else {
            console.log("sucessfully updated plant entry! res: " + res);
        }
});
}

app.listen(port, () => {
    console.log(`RemoteGardeningBackend listening at http://localhost:${port}`);
});