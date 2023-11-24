// CaptivePortalHTML.h
#ifndef CaptivePortalHTML_h
#define CaptivePortalHTML_h

#include <Arduino.h>

const char captivePortalHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP8266 Wi-Fi Connect</title>
    <style>
        .status {
            color: black;
        }
        .connecting {
            color: blue;
        }
        .success {
            color: green;
        }
        .error {
            color: red;
        }
    </style>
    <script>
        function scanNetworks() {
            fetch('/scan')
                .then(response => response.json())
                .then(data => {
                    const networkList = document.getElementById('networkList');
                    networkList.innerHTML = ''; // Clear existing list

                    data.forEach((net, index) => {
                        const radioItem = document.createElement('input');
                        radioItem.type = 'radio';
                        radioItem.id = 'net' + index;
                        radioItem.name = 'network';
                        radioItem.value = net.SSID;

                        const label = document.createElement('label');
                        label.htmlFor = 'net' + index;
                        label.textContent = net.SSID;

                        const lineBreak = document.createElement('br');

                        networkList.appendChild(radioItem);
                        networkList.appendChild(label);
                        networkList.appendChild(lineBreak);
                    });
                })
                .catch(error => {
                    console.error('There has been a problem with your fetch operation:', error);
                    alert('Error: ' + error.message);
                });
        }

        function connect() {
            const selectedNetwork = document.querySelector('input[name="network"]:checked');
            if (!selectedNetwork) {
                alert('Please select a network');
                return;
            }

            const ssid = selectedNetwork.value;
            const password = document.getElementById('password').value;
            const statusLabel = document.getElementById('status');

            statusLabel.innerText = 'Connecting...';
            statusLabel.className = 'status connecting';

            fetch('/connect', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`
            })
            .then(response => {
                if (response.ok) {
                    statusLabel.innerText = 'Connected successfully';
                    statusLabel.className = 'status success';
                    setTimeout(() => {
                        window.close(); // Close the captive portal window
                    }, 2000); // Close after 2 seconds to allow user to see the message
                } else {
                    statusLabel.innerText = 'Connection failed';
                    statusLabel.className = 'status error';
                }
            })
            .catch(error => {
                statusLabel.innerText = 'Connection failed';
                statusLabel.className = 'status error';
            });
        }

        window.onload = scanNetworks;
    </script>
</head>
<body>
    <h1>ESP8266 Wi-Fi Connect</h1>
    <div id="networkList"></div> <!-- Network list will be populated here -->
    <button onclick="scanNetworks()">Refresh Networks</button><br><br>
    Password: <input type="password" id="password"><br><br>
    <button onclick="connect()">Connect</button><br><br>
    <label id="status" class="status">Status: Idle</label>
</body>
</html>
)rawliteral";

#endif