// IrrigatorConfigHTML.h
#ifndef IrrigatorConfigHTML_h
#define IrrigatorConfigHTML_h

#include <Arduino.h>

const char irrigatorConfigHTML[] PROGMEM = R"rawliteral(
<html>
<head>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background-color: #f4f4f4;
        }
        .container {
            width: 60%;
            padding: 20px;
            background: #fff;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            border-radius: 5px;
        }
        h1 {
            color: #333;
            text-align: center;
        }
        h2 {
            color: #333;
            text-align: left;
            margin-top: 20px;
        }
        .form-group {
            margin-bottom: 15px;
            text-align: left;
        }
        input[type='text'], input[type='password'], input[type='submit'], button {
            width: 100%;
            padding: 10px;
            margin-top: 5px;
            margin-bottom: 5px;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box; /* Fixes padding and border issues */
        }
        .button-group {
            display: flex;
            justify-content: space-between;
            flex-wrap: wrap; /* Wrap buttons if space is not sufficient */
        }
        button, input[type='submit'] {
            background-color: #007bff;
            color: white;
            cursor: pointer;
            border: none;
            transition: background-color 0.3s;
        }
        button:hover, input[type='submit']:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Irrigator App</h1>
        <div id='wifiSSID' class="form-group"></div>

        <h2>MQTT Configuration</h2>
        <form action='/mqtt' method='post' class="form-group">
            Server: <input type='text' id='mqttServer' name='server'><br>
            Port: <input type='text' id='mqttPort' name='port'><br>
            Username: <input type='text' id='mqttUsername' name='username'><br>
            Password: <input type='password' id='mqttPassword' name='password'><br>
            Status Topic: <input type='text' id='mqttStatusTopic' name='status_topic'><br>
        </form>
        
        <div class="button-group">
            <button onclick="saveConfig()">Save Changes</button>
            <form action='/reboot' method='post' class="form-group">
                <input type='submit' value='Reboot' class='button'>
            </form>
            <form action='/restore' method='post' class="form-group">
                <input type='submit' value='Restore Defaults' class='button'>
            </form>
            <form action='/wifi/forget' method='post' class="form-group">
                <input type='submit' value='Forget Wi-Fi' class='button'>
            </form>
        </div>
    </div>

    <script>
        fetch('/wifi').then(response => response.json()).then(data => {
            document.getElementById('wifiSSID').innerText = data.ssid;
        }).catch(error => console.error('Error:', error));

        fetch('/mqtt').then(response => response.json()).then(data => {
            document.getElementById('mqttServer').value = data.server || '';
            document.getElementById('mqttPort').value = data.port || '';
            document.getElementById('mqttUsername').value = data.username || '';
            document.getElementById('mqttPassword').value = data.password || '';
            document.getElementById('mqttStatusTopic').value = data.status_topic || '';
        }).catch(error => console.error('Error:', error));

        function saveConfig() {
            const server = document.getElementById('mqttServer').value;
            const port = document.getElementById('mqttPort').value;
            const username = document.getElementById('mqttUsername').value;
            const password = document.getElementById('mqttPassword').value;
            const statusTopic = document.getElementById('mqttStatusTopic').value;

            fetch('/mqtt', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `server=${encodeURIComponent(server)}&port=${encodeURIComponent(port)}&username=${encodeURIComponent(username)}&password=${encodeURIComponent(password)}&status_topic=${encodeURIComponent(statusTopic)}`
            })
            .then(response => {
                if (response.ok) {
                    alert('Configuration saved successfully');
                } else {
                    alert('Failed to save configuration');
                }
            })
            .catch(error => {
                alert('Error: ' + error.message);
            });
        }
    </script>
</body>
</html>
)rawliteral";

#endif
