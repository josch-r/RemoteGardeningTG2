const express = require("express");
const axios = require("axios");
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

app.use(express.json());
app.get("/", (req, res) => {
    res.send("service is alive");
});

app.post("/plant/register", async(req, res) => {
    let rfidUID = req.body.uid;
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
});
app.get("/plant/status/:uid", async(req, res) => {
    let rfidUID = req.params.uid;
    console.log(rfidUID);
    //get mongoDB entry for UID
    plantsCollection.findOne({ rfidUID: rfidUID }, (err, result) => { //TODO nsyntax
        if (err) throw err;
        if (result !== null) {
            console.log(result);
            let waterMS = 0;
            switch (result.waterLevel) {
                case 1:
                    waterMS = 3000;
                    break;
                case 2:
                    waterMS = 4500;
                    break;
                case 3:
                    waterMS = 6000;
                    break;
                case 4:
                    waterMS = 7500;
                    break;
                case 5:
                    waterMS = 9000;
                    break;
            }
            res.status(200).send(waterMS.toString()).end();
        } else {
            res.status(404).send("plant doesnt exist in DB").end();
        }
    });
});

app.listen(port, () => {
    console.log(`RemoteGardeningBackend listening at http://localhost:${port}`);
});