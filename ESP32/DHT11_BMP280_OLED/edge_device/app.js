const express = require('express')
const app = express();
var bodyParser = require('body-parser');

const port = 80

app.use(express.urlencoded({
    extended: true
}));

app.use(bodyParser.json());


app.get('/', (req, res) => {

    console.log(req.params)

})

app.post('/BME280', (req, res) => {
    console.log(req.headers["node-name"])
    console.log(req.body)
    console.log(req.headers)
})

app.post('/DHT', (req, res) => {
    console.log(req.headers)
    console.log(req.headers["node-name"])
    console.log(req.body)
})


app.post('/water-sensors', (req, res) => {
    console.log(req.headers)
    console.log(req.headers["node-name"])
    console.log(req.body)
})


app.listen(port, () => console.log(`Example app listening on port ${port}!`))