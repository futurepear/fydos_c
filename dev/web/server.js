const http = require('http');
const fs = require("fs");
console.log("started");
const server = http.createServer((req, res) => {

    const headers = {
        'Access-Control-Allow-Origin': '*', /* @dev First, read about security */
        'Access-Control-Allow-Methods': 'OPTIONS, POST, GET',
        'Access-Control-Max-Age': 2592000, // 30 days
        /** add other headers as per requirement */
    };  

    if (req.method == 'OPTIONS') {
        res.writeHead(204, headers);
        return res.end();
    }

    

    if (req.url === "/dictionary.cpp" && req.method == "POST") {
        
        let body = '';
        req.on('data', (chunk) => { body += chunk.toString();  });
        req.on('end', () => {
            let dictionary_cpp = fs.readFileSync('../../game/systems/dictionary.cpp', { encoding: 'utf8', flag: 'r' });
            let file = dictionary_cpp.split("//__ITEMSTART__")[0] + "//__ITEMSTART__" + body + "//__ITEMEND__" + dictionary_cpp.split("//__ITEMEND__")[1];
            
            fs.writeFile('../../game/systems/dictionary.cpp', file, 'utf8', (err) => {
                if (err) {
                    return console.log('ERROR');
                }
            });
        });
    }

    if (req.url === '/dictionary.cpp' && req.method === 'GET') {
        fs.readFile('../../game/systems/dictionary.cpp', 'utf8', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain', ...headers });
                res.end('noob');
            } else {
                res.writeHead(200, { 'Content-Type': 'text/plain', ...headers });
                dictionary_cpp = data;
                res.end(data);
                
            }
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Route not found');
    }
});

server.listen(3000);
