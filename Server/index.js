const express = require('express');
const app = express();
const websocket = require('express-ws')(app);

app.ws('/', function(wss, req) {
    console.log("[info][websocket] New connection has opened!");
    wss.on('message', data => {
        console.log("[info][websocket] Recieved data: ", data);
        data = JSON.parse(data);
        //console.log(data);
        switch (data.type) {
            case "connection":
                console.log(`[info][websocket] Client: ${data.client}, ID: ${data.id}`);
                break;
            case "action":
                websocket.getWss().clients.forEach(client => {
                    client.send(JSON.stringify(data));
                })
                //wss.send(JSON.stringify(data));
                console.log('[info][websocket] Action sended to client!', JSON.stringify(data));
                break;
            default:
                console.log('[info][websocket] Data recieved', data);
                break;
        }
    });

    wss.on('close', () => {
        console.log('[info][websocket] Client connection closed');
    });
});

app.listen(3001, function() {
    console.log('[info][express] Server has been started')
});