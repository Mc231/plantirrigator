#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include "WiFiConfigManager.h"
#include "LittleFSImpl.h"

LittleFSImpl fileSystem;
WiFiConfigManager configManager(fileSystem);

const char* ap_ssid = "ESP8266-AP";
const char* ap_password = "123456789";

ESP8266WebServer server(80);
DNSServer dnsServer;


void handleScanNetworks();
void handleConnectToNetwork();
void handleCaptivePortal();
void saveConfig(const String& ssid, const String& password, const String& forceSetup);
void startAPMode();

struct Config {
    String ssid;
    String password;
    bool forceSetup;
};

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
            const select = document.getElementById('networks');
            select.innerHTML = '<option value="">Select a network</option>'; // Clear existing options and add default

            data.forEach(net => {
                const option = document.createElement('option');
                option.value = net.SSID;
                option.textContent = net.SSID;
                select.appendChild(option);
            });
        })
        .catch(error => {
            console.error('There has been a problem with your fetch operation:', error);
            alert('Error: ' + error.message);
        });
}

        function connect() {
            const ssid = document.getElementById('networks').value;
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
        if(response.ok) {
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
    </script>
</head>
<body onload="scanNetworks()">
    <h1>ESP8266 Wi-Fi Connect</h1>
    <label for="networks">Networks:</label>
    <select id="networks">
        <option value="">Select a network</option>
    </select><br><br>
    Password: <input type="password" id="password"><br><br>
    <button onclick="connect()">Connect</button><br><br>
    <label id="status" class="status">Status: Idle</label>
</body>
</html>
)rawliteral";


void startAPMode() {
    Serial.println("Start AP mode");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid, ap_password);
    dnsServer.start(53, "*", WiFi.softAPIP());
    server.on("/scan", HTTP_GET, handleScanNetworks);
    server.on("/connect", HTTP_POST, handleConnectToNetwork);
    server.onNotFound(handleCaptivePortal);
    Serial.println("End AP mode");
    server.begin();
}




void setup() {
    Serial.begin(115200);
    WiFiConfigManager::Config config = configManager.readConfig();
    Serial.print("SSID: '");
    Serial.print(config.ssid);
    Serial.println("'");
    Serial.print("Password: '");
    Serial.print(config.password);
    Serial.println("'");
    if (!config.ssid.isEmpty() && !config.password.isEmpty() && !config.forceSetup) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(config.ssid, config.password);

        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("Failed to connect. Entering setup mode...");
            startAPMode();
        } else {
            Serial.println("Connected to Wi-Fi. Operating in normal mode.");
            // Add normal mode operations here
        }
    } else {
        startAPMode();
    }
}



void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
}

void handleScanNetworks() {
    String json = "[";
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        if (i) json += ",";
        json += "{\"SSID\":\"" + WiFi.SSID(i) + "\",\"RSSI\":" + WiFi.RSSI(i) + "}";
    }
    json += "]";
    server.send(200, "application/json", json);
    WiFi.scanDelete();
}

void handleConnectToNetwork() {
    if (server.hasArg("ssid") && server.hasArg("password")) {
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        WiFi.begin(ssid.c_str(), password.c_str());

        int count = 0;
        while (WiFi.status() != WL_CONNECTED && count < 30) {
            delay(1000);
            count++;
            Serial.print(".");
        }
        
    if (WiFi.status() == WL_CONNECTED) {
        server.send(200, "text/plain", "Connected to " + ssid);
        saveConfig(ssid, password, "false"); // Save SSID, PASSWORD, and set FORCE_SETUP to false
        delay(1000); // Optional delay
        ESP.restart(); // Optional: Restart the ESP8266
    } else {
        server.send(504, "text/plain", "Connection failed");
    }
    } else {
        server.send(400, "text/plain", "Missing ssid or password");
    }
}

void saveConfig(const String& ssid, const String& password, const String& forceSetup) {
  configManager.saveConfig(ssid, password, "false");
}

void handleCaptivePortal() {
    server.send(200, "text/html", captivePortalHTML);
}