#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <EEPROM.h>

// Definisci la struttura
struct Data {
  char device_id[37];  // 36 caratteri + 1 per il carattere null
  char api_key[33];  // 32 caratteri + 1 per il carattere null
  bool isApOn;  // Se l'AP è acceso
  char apName[33];  // Nome dell'AP
  char apPassword[65];  // Password dell'AP
  char wifiSsid[33];  // SSID della rete WiFi
  char wifiPassword[65];  // Password della rete WiFi
};

// Crea un'istanza della struttura
Data data;

void saveData() {
  // Salva la struttura nella EEPROM
  EEPROM.put(0, data);
  EEPROM.commit();
}

void loadData() {
  // Leggi i dati dalla EEPROM
  memset(&data, 0, sizeof(Data));
  EEPROM.get(0, data);
}

void refreshData() {
  saveData();
  loadData();
}

void resetData() {
  // Imposta tutti i campi della struttura a valori di default
  memset(&data, 0, sizeof(data));

  // Salva la struttura nella EEPROM
  saveData();
}

// Definisci le costanti
const char *apSSID = data.apName;  // Nome dell'access point
const char *apPassword = data.apPassword;  // Password dell'access point
const char *ssidKey = data.wifiSsid;  // Chiave per la rete WiFi
const char *passwordKey = data.wifiPassword;  // Chiave per la password della rete WiFi
const String baseUrl = "https://ecoplant-back.yirade.dev/";

const String svgIcon = R"=====(
  <svg class="logo-svg" version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 1000 1000" enable-background="new 0 0 1000 1000" xml:space="preserve">
          <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
	 width="100%" viewBox="0 0 1000 1000" enable-background="new 0 0 1000 1000" xml:space="preserve">
<path fill="#FFFFFF" opacity="1.000000" stroke="none" 
	d="
M458.000000,1001.000000 
	C305.333344,1001.000000 153.166672,1001.000000 1.000000,1001.000000 
	C1.000000,667.666687 1.000000,334.333344 1.000000,1.000000 
	C334.333344,1.000000 667.666687,1.000000 1001.000000,1.000000 
	C1001.000000,334.333344 1001.000000,667.666687 1001.000000,1001.000000 
	C820.166687,1001.000000 639.333313,1001.000000 458.000000,1001.000000 
M535.613892,851.105652 
	C545.057617,843.719421 554.541382,836.383667 563.938171,828.938171 
	C607.404419,794.498352 651.267273,760.531921 692.079651,722.890320 
	C713.609314,703.033142 733.162231,681.479736 747.160583,655.493408 
	C774.469299,604.798279 778.323792,551.258118 765.631348,495.951263 
	C755.329834,451.062286 735.405823,410.385803 709.996521,372.276459 
	C686.110291,336.451233 658.556396,303.439667 630.453613,270.936371 
	C593.091064,227.723236 555.125244,185.031647 517.399048,142.133118 
	C512.053528,136.054764 506.598328,130.072937 500.701904,123.497856 
	C499.662506,124.867447 499.021454,125.838272 498.257599,126.700287 
	C480.695526,146.520264 462.877350,166.119308 445.596191,186.181000 
	C406.254364,231.852966 366.778137,277.419220 328.115601,323.663086 
	C297.767700,359.961945 271.849365,399.275909 253.550476,443.190277 
	C237.423767,481.891785 227.870285,521.909180 229.795273,564.189331 
	C232.028244,613.234375 249.322281,656.239929 281.293396,693.414062 
	C303.886871,719.684509 330.254486,741.957642 356.969391,763.739258 
	C394.268494,794.150574 432.128693,823.874268 469.786377,853.845032 
	C480.054199,862.016907 490.456696,870.019592 501.011536,878.267273 
	C512.557434,869.209229 523.808716,860.382324 535.613892,851.105652 
z"/>
<path fill="#69BD3F" opacity="1.000000" stroke="none" 
	d="
M535.336914,851.330566 
	C523.808716,860.382324 512.557434,869.209229 501.011536,878.267273 
	C490.456696,870.019592 480.054199,862.016907 469.786377,853.845032 
	C432.128693,823.874268 394.268494,794.150574 356.969391,763.739258 
	C330.254486,741.957642 303.886871,719.684509 281.293396,693.414062 
	C249.322281,656.239929 232.028244,613.234375 229.795273,564.189331 
	C227.870285,521.909180 237.423767,481.891785 253.550476,443.190277 
	C271.849365,399.275909 297.767700,359.961945 328.115601,323.663086 
	C366.778137,277.419220 406.254364,231.852966 445.596191,186.181000 
	C462.877350,166.119308 480.695526,146.520264 498.257599,126.700287 
	C499.021454,125.838272 499.662506,124.867447 500.701904,123.497856 
	C506.598328,130.072937 512.053528,136.054764 517.399048,142.133118 
	C555.125244,185.031647 593.091064,227.723236 630.453613,270.936371 
	C658.556396,303.439667 686.110291,336.451233 709.996521,372.276459 
	C735.405823,410.385803 755.329834,451.062286 765.631348,495.951263 
	C778.323792,551.258118 774.469299,604.798279 747.160583,655.493408 
	C733.162231,681.479736 713.609314,703.033142 692.079651,722.890320 
	C651.267273,760.531921 607.404419,794.498352 563.938171,828.938171 
	C554.541382,836.383667 545.057617,843.719421 535.336914,851.330566 
M454.144836,345.609436 
	C435.313507,351.661835 418.255676,360.977783 404.050446,374.759521 
	C368.603363,409.149872 359.976410,451.279205 369.717255,498.264893 
	C374.049683,519.162659 385.396118,536.649353 398.160065,553.270081 
	C405.150177,562.372314 410.498199,571.718994 411.430878,583.814331 
	C412.250885,594.448364 417.453979,602.286682 431.862549,601.945374 
	C458.174194,601.322144 484.510925,601.758667 510.837189,601.753052 
	C533.164551,601.748352 555.491943,601.778320 577.819153,601.723633 
	C588.390503,601.697754 594.728943,596.723877 596.943481,586.428528 
	C597.739929,582.725708 597.973816,578.824158 597.892212,575.025635 
	C597.693481,565.786926 600.416992,557.726990 606.286621,550.566101 
	C610.077576,545.941284 614.007568,541.296448 616.953918,536.134705 
	C639.143860,497.260498 642.444092,456.644379 625.893127,415.107758 
	C611.299316,378.482880 584.373352,354.408447 546.481995,343.478546 
	C515.920349,334.662964 485.248993,336.067261 454.144836,345.609436 
M587.819275,662.536438 
	C597.340271,655.519836 600.140198,641.044373 594.241272,629.640686 
	C591.296204,623.947327 586.761780,620.888428 580.288208,620.891602 
	C529.999451,620.916504 479.710663,620.963623 429.421875,620.956726 
	C421.878693,620.955627 416.383362,624.415100 413.925568,631.282349 
	C412.295776,635.836182 411.311829,641.082458 411.790955,645.842407 
	C413.177673,659.619324 419.282562,664.715881 433.042023,664.709900 
	C481.498962,664.688843 529.955994,664.645996 578.412598,664.494141 
	C581.324402,664.485046 584.232971,663.453430 587.819275,662.536438 
M521.756714,725.337097 
	C544.425232,719.473816 555.270691,703.103394 560.911499,681.742554 
	C523.332275,681.742554 486.168365,681.742554 448.969635,681.742554 
	C449.997925,699.356567 465.231781,718.288940 482.558411,724.005676 
	C495.202026,728.177307 508.027313,728.414917 521.756714,725.337097 
z"/>
<path fill="#FEFFFE" opacity="1.000000" stroke="none" 
	d="
M454.518311,345.466217 
	C485.248993,336.067261 515.920349,334.662964 546.481995,343.478546 
	C584.373352,354.408447 611.299316,378.482880 625.893127,415.107758 
	C642.444092,456.644379 639.143860,497.260498 616.953918,536.134705 
	C614.007568,541.296448 610.077576,545.941284 606.286621,550.566101 
	C600.416992,557.726990 597.693481,565.786926 597.892212,575.025635 
	C597.973816,578.824158 597.739929,582.725708 596.943481,586.428528 
	C594.728943,596.723877 588.390503,601.697754 577.819153,601.723633 
	C555.491943,601.778320 533.164551,601.748352 510.837189,601.753052 
	C484.510925,601.758667 458.174194,601.322144 431.862549,601.945374 
	C417.453979,602.286682 412.250885,594.448364 411.430878,583.814331 
	C410.498199,571.718994 405.150177,562.372314 398.160065,553.270081 
	C385.396118,536.649353 374.049683,519.162659 369.717255,498.264893 
	C359.976410,451.279205 368.603363,409.149872 404.050446,374.759521 
	C418.255676,360.977783 435.313507,351.661835 454.518311,345.466217 
z"/>
<path fill="#FCFEFB" opacity="1.000000" stroke="none" 
	d="
M587.481201,662.717041 
	C584.232971,663.453430 581.324402,664.485046 578.412598,664.494141 
	C529.955994,664.645996 481.498962,664.688843 433.042023,664.709900 
	C419.282562,664.715881 413.177673,659.619324 411.790955,645.842407 
	C411.311829,641.082458 412.295776,635.836182 413.925568,631.282349 
	C416.383362,624.415100 421.878693,620.955627 429.421875,620.956726 
	C479.710663,620.963623 529.999451,620.916504 580.288208,620.891602 
	C586.761780,620.888428 591.296204,623.947327 594.241272,629.640686 
	C600.140198,641.044373 597.340271,655.519836 587.481201,662.717041 
z"/>
<path fill="#FCFEFA" opacity="1.000000" stroke="none" 
	d="
M521.363525,725.469482 
	C508.027313,728.414917 495.202026,728.177307 482.558411,724.005676 
	C465.231781,718.288940 449.997925,699.356567 448.969635,681.742554 
	C486.168365,681.742554 523.332275,681.742554 560.911499,681.742554 
	C555.270691,703.103394 544.425232,719.473816 521.363525,725.469482 
z"/>
</svg>
        </svg>
        )=====";

const String styles = R"=====(
  <style>
        body {
        background-color: #f0f0f0;
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 0;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
      }

      .container {
        display: flex;
        align-items: center;
        max-width: 750px;
        margin: 0 auto;
        padding: 25px;
        background-color: #fff;
        border-radius: 14px;
        position: relative;
        box-shadow: rgba(0, 0, 0, 0.075) 0px 2px 4px 0px;
      }

      .logo-container {
        display: flex;
        justify-content: center;
        align-items: center;
        margin: 0 auto;
        position: absolute;
        top: 10px;
        left: 0;
      }

      .logo-svg {
        width: 100px;
        height: auto;
      }
      
      h1 {
        color: #333;
        text-align: center;
        margin-right: 20px;
      }

      form {
        margin-top: 100px;
        margin-left: 250px;
        margin-right: 25px;
        margin-bottom: 50px;
      }

      p {
        position: absolute;
        bottom: 10px;
        left: 30px;
      }

      a {
        color: #4CAF50;
        text-decoration: none;
      }

      input[type='submit'] {
        background-color: #4CAF50;
        color: white;
        padding: 14px 20px;
        margin: 8px 0;
        border: none;
        cursor: pointer;
        width: 100%;
        border-radius: 14px;
        margin-top: 20px;
      }

      input[type='submit']:hover {
        opacity: 0.8;
      }

      input[type='text'], input[type='password'], input[type='email'] {
        
                font-size: 14px;
                border-radius: 6px;
                line-height: 1.5;
                padding: 5px 10px;
                transition: box-shadow 100ms ease-in, border 100ms ease-in, background-color 100ms ease-in;
                border: 2px solid #dee1e2;
                color: rgb(14, 14, 16);
                background: #dee1e2;
                display: block;
                height: 30px;
                margin-top: 2px;
                :hover {
                    border-color: #ccc;
                }
                :focus{
                    border-color: #9147ff;
                    background: #fff;
                }
                
      }

      input[type='radio'] {
        margin-right: 10px;
        margin-bottom: 2px;
      }

      /* CSS per visualizzare le barre del segnale a destra dei radio button */
      .wifi-bar {
        background-color: #ddd;
        height: 20px;
        margin-bottom: 5px;
        border-radius: 10px;
        margin-top: 2px;
        overflow: hidden;
      }

      .wifi-bar-fill {
        background-color: #4CAF50;
        height: 100%;
      }

      .title {
        margin-left: 25px;
      }
      </style>
)=====";


void saveStringToCharArray(char* charArray, const String& str, size_t size) {
  strncpy(charArray, str.c_str(), size);
  charArray[size - 1] = '\0';  // Assicurati che la stringa sia terminata correttamente
  Serial.print("Stringa salvata: ");
  Serial.println(charArray);
}

Preferences preferences;

WebServer server(80);

void handleWifi()
{
  String content = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    )=====";
  content += styles;
  content += R"=====(
    <title>WiFi - EcoPlant</title>
  </head>
  <body>
    <div class="container">
     <div class="logo-container">
     )=====";
  content += svgIcon;
  content += R"=====(
        <!-- Fine icona SVG -->
        <h1>EcoPlant</h1> <!-- Testo del logo -->
      </div>
      <h1 class="title">Reti WiFi Disponibili:</h1>
  )=====";

  int numNetworks = WiFi.scanNetworks();

  Serial.println(numNetworks);
  if (numNetworks == 0)
  {
    content += "<span>Nessuna rete WiFi trovata</span>";
  } else if (numNetworks < 0)
  {
    content += "<span>Errore durante la scansione delle reti WiFi</span>";
  } 
  else
  {
    content += "<form method='get' action='/connect'>";
    for (int i = 0; i < numNetworks; ++i)
    {
      content += "<label>";
      content += "<input type='radio' name='ssid' value='" + WiFi.SSID(i) + "'>";
      content += WiFi.SSID(i);
      content += "<div class='wifi-bar'><div class='wifi-bar-fill' style='width: " + String(WiFi.RSSI(i) + 100) + "%;'></div></div>";
      content += "</label><br>";
    }
    content += "<label for='password'>Password (se necessaria):</label><br>";
    content += "<input type='text' name='password' placeholder='password'><br>";
    content += "<input type='submit' value='Connetti'>";
    content += "</form>";
  }

  content += R"=====(
    </div>
  </body>
  </html>
  )=====";

  server.send(200, "text/html", content);
}

void handleConnect()
{
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  Serial.print("Connettendo a ");
  Serial.println(ssid);

  WiFi.disconnect();
  
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10)
  {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connesso con successo!");
    saveStringToCharArray(data.wifiSsid, ssid, sizeof(data.wifiSsid));
    saveStringToCharArray(data.wifiPassword, password, sizeof(data.wifiPassword));
    refreshData();
    server.sendHeader("Location", "/login");
    server.send(302, "text/plain", "");
  }
  else
  {
    Serial.println("Connessione fallita");
    server.send(200, "text/html", "<h1>Connessione fallita</h1>");
  }
}

void handleLogin()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    server.sendHeader("Location", "/wifi");
    server.send(302, "text/plain", "");
    return;
  }

  String content = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Login - EcoPlant</title>
      <!-- Includi eventuali stili CSS necessari -->
      )=====";
  content += styles;
  content += R"=====(
    </head>
    <body>
    <div class="container">
    <div class="logo-container">
        )=====";
  content += svgIcon;
  content += R"=====(
        <!-- Fine icona SVG -->
        <h1>EcoPlant</h1> <!-- Testo del logo -->
      </div>
      <h1 class="title">Login:</h1>
      <form method='post' action='/login'>
        <label for='username'>Username:</label><br>
        <input type='text' name='username' required><br>
        <label for='password'>Password:</label><br>
        <input type='password' name='password' required><br>
        <input type='submit' value='Login'>
      </form>
      <p>Non hai un account EcoPlant? <a href='/register'>Registrati qui</a></p>
    </div>
    </body>
    </html>
  )=====";

  server.send(200, "text/html", content);
}

void handleRegister()
{
  String content = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Registrazione - EcoPlant</title>
      <!-- Includi eventuali stili CSS necessari -->
      )=====";	
  content += styles;
  content += R"=====(
    </head>
    <body>
    <div class="container">
    <div class="logo-container">
         )=====";
  content += svgIcon;
  content += R"=====(
        <!-- Fine icona SVG -->
        <h1>EcoPlant</h1> <!-- Testo del logo -->
      </div>
      <h1 class="title">Registrazione</h1>
      <form method='post' action='/register'>
        <label for='username'>Username:</label><br>
        <input type='text' name='username' required><br>
        <label for='email'>Email:</label><br>
        <input type='email' name='email' required><br>
        <label for='password'>Password:</label><br>
        <input type='password' name='password' required><br>
        <input type='submit' value='Registrati'>
      </form>
      <p>Hai gia un account EcoPlant? <a href='/login'>Effettua il login qui</a></p>
    </div>
    </body>
    </html>
  )=====";

  server.send(200, "text/html", content);
}

void handleLoginSubmit()
{
  String username = server.arg("username");
  String password = server.arg("password");

  // Esempio di invio dei dati al backend per il login
  HTTPClient http;
  http.begin(baseUrl + "api/v1/login");
  http.addHeader("Content-Type", "application/json");

  // Crea il JSON da inviare
  StaticJsonDocument<200> json;
  json["username"] = username;
  json["password"] = password;

  // Serializza il JSON in una stringa
  String jsonString;
  serializeJson(json, jsonString);

  // Invia la richiesta POST con i dati di login
  int httpResponseCode = http.POST(jsonString);

  // Gestisce la risposta dal backend
  if (httpResponseCode == 200)
  {
    // Ricevi e gestisci i dati di accesso
    String response = http.getString();
    StaticJsonDocument<200> jsonResponse;
    deserializeJson(jsonResponse, response);

    String accessToken = jsonResponse["access"];

    // Utilizza l'accessToken per effettuare altre richieste al backend
  }
  else
  {
    // Gestisci eventuali errori di login
    server.send(401, "text/plain", "Login fallito");
  }

  http.end();
}

void handleRegisterSubmit()
{
  String username = server.arg("username");
  String email = server.arg("email");
  String password = server.arg("password");

  // Esempio di invio dei dati al backend per la registrazione
  HTTPClient http;
  http.begin(baseUrl + "api/v1/register");
  http.addHeader("Content-Type", "application/json");

  // Crea il JSON da inviare
  StaticJsonDocument<200> json;
  json["username"] = username;
  json["email"] = email;
  json["password"] = password;

  // Serializza il JSON in una stringa
  String jsonString;
  serializeJson(json, jsonString);

  // Invia la richiesta POST con i dati di registrazione
  int httpResponseCode = http.POST(jsonString);

  // Gestisce la risposta dal backend
  if (httpResponseCode == 200)
  {
    // Ricevi e gestisci i dati di accesso
    String response = http.getString();
    StaticJsonDocument<200> jsonResponse;
    deserializeJson(jsonResponse, response);

    String accessToken = jsonResponse["access"];

    // Utilizza l'accessToken per effettuare altre richieste al backend
  }
  else
  {
    // Gestisci eventuali errori di registrazione
    server.send(400, "text/plain", "Registrazione fallita");
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Inizializzazione...");

  // Inizializza la EEPROM
  EEPROM.begin(sizeof(data));
  Serial.println("EEPROM inizializzata");
  // Carica i dati dalla EEPROM
  loadData();
  // stampa i dayi caricati dalla EEPROM
  Serial.print("Dati caricati dalla EEPROM: ");
  Serial.print("device_id: ");
  Serial.print(data.device_id);
  Serial.print(" api_key: ");
  Serial.print(data.api_key);
  Serial.print(" isApOn: ");
  Serial.print(data.isApOn);
  Serial.print(" apName: ");
  Serial.print(data.apName);
  Serial.print(" apPassword: ");
  Serial.print(data.apPassword);
  Serial.print(" wifiSsid: ");
  Serial.print(data.wifiSsid);
  Serial.print(" wifiPassword: ");
  Serial.print(data.wifiPassword);

  Serial.println("Dati caricati dalla EEPROM");

  // Connessione alla rete WiFi
  Serial.println("Connessione alla rete WiFi...");
  WiFi.mode(WIFI_AP_STA);
  Serial.print("Connessione in corso");
  if (String(data.wifiSsid) != "" && String(data.wifiPassword) != "")
  {
    // Connessione alla rete WiFi
    WiFi.begin(data.wifiSsid, data.wifiPassword);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
      delay(1000);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnesso con successo alla rete WiFi");
    }
    else
    {
      // disattiva la connessione alla rete wifi
      WiFi.disconnect();
      Serial.println("\nConnessione alla rete WiFi fallita");
    }
  }
  else
  {
    Serial.println("Nessuna rete WiFi salvata");
  }

  // Connessione all'access point ESP32
  Serial.println("Connessione all'access point ESP32...");
  if (data.isApOn)
  {
    if (String(data.apName) != "" && String(data.apPassword) != "")
    {
      WiFi.softAP(data.apName, data.apPassword);
      IPAddress IP = WiFi.softAPIP();
      Serial.print("Indirizzo IP access point: ");
      Serial.println(IP);
    }
    else // Se non ci sono dati salvati per l'access point, crea un access point con nome e password di default
    {
      WiFi.softAP("EcoPlant", "");
      IPAddress IP = WiFi.softAPIP();
      Serial.print("Indirizzo IP access point: ");
      Serial.println(IP);
    } // se non sono connesso a nessun wifi e data.isApOn e false allora crea un access point con nome e password di default
  } else if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.softAP("EcoPlant", "");
    saveStringToCharArray(data.apName, "EcoPlant", sizeof(data.apName));
    saveStringToCharArray(data.apPassword, "", sizeof(data.apPassword));
    data.isApOn = true;
    refreshData();
    IPAddress IP = WiFi.softAPIP();
    Serial.print("Indirizzo IP access point: ");
    Serial.println(IP);
  }
  else
  {
    Serial.println("Access point ESP32 non connesso");
  }

  server.on("/", handleLogin);
  server.on("/wifi", handleWifi);
  server.on("/connect", handleConnect);
  server.on("/login", HTTP_POST, handleLoginSubmit);
  server.on("/register", HTTP_POST, handleRegisterSubmit);
  server.on("/login", handleLogin);
  server.on("/register", handleRegister);

  server.begin();
  Serial.println("Server avviato");
}

void loop()
{
  server.handleClient();
}
