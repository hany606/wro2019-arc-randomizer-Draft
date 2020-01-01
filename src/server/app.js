import * as path from 'path';
import express from 'express';
import generateField from './generate_field';

var app = express();

const routes = express.Router();

routes.get('/field', async function(req, res, next) {
    let field = (await generateField());
    res.send(field);
    next();
});

console.log(path.join('public'));
routes.use('/',express.static(path.join('public')));

app.use('/', routes);

const port = process.env.PORT || 3000;
// Fire it up!
app.listen(port);
console.log('Listening on port ' + port);
