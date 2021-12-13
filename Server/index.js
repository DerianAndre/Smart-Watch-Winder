const express = require('express');
const app = express();
const websocket = require('express-ws')(app);

app.ws('/', function(wss, req) {
    console.log("[info][websocket] New connection has opened!");
    //console.log(websocket.getWss().clients);
    wss.on('connection', function connection(ws, req) {
        // TODO: Add ID to WebSocketServer client list
        //ws.id = wss.getUniqueID();
        wss.clients.forEach(function each(client) {
            //console.log('Client.ID: ' + client.id);
        });
    });

    wss.on('message', data => {
        console.log("[info][websocket] Recieved data:", data);
        data = JSON.parse(data);
        //console.log(data);
        switch (data.type) {
            case "connection":
                console.log(`[info][websocket] Client: ${data.client}, ID: ${data.id}`);
                console.log(`[info][websocket] Requesting data to ESP32`);
                if(data.client == "frontend") {
                    websocket.getWss().clients.forEach(client => {
                        if(client.readyState === 1) {
                            client.send(JSON.stringify({
                                type: "request",
                                client: "esp32",
                                request: "settings",
                            }));
                        }
                    });
                }
                break;
            case "action":
                websocket.getWss().clients.forEach(client => {
                    client.send(JSON.stringify(data));
                })
                console.log('[info][websocket] Action sended to client!', JSON.stringify(data));
                break;
            case "settings":
                console.log('[info][websocket] Settings recieved from ESP32!', JSON.stringify(data));
                websocket.getWss().clients.forEach(client => {
                    client.send(JSON.stringify(data));
                });
                break;
            default:
                console.log('[info][websocket] Data recieved:', JSON.stringify(data));
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