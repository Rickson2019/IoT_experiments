const express = require('express');
const app = express();
const port = 3000;

var bodyParser = require('body-parser');
// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: true }));

// parse application/json
app.use(bodyParser.json());


app.get('/', (req, res) => res.send('Hello World!'))


app.post('/outdoor_1', (req, res) => {
    try {
        console.log(Date.now());

        console.log(req.body);

    } catch (e) {
        console.error(e);
    }

})


app.listen(port, () => console.log(`Example app listening on port ${port}!`))