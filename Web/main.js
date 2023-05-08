/* --------------------------- CONFIG FIREBASE ------------------------------- */
// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
    apiKey: "AIzaSyDv7Ib8BkKSXGSScJ0ZSAPVF1lZtXobdgw",
    authDomain: "datn-lora.firebaseapp.com",
    databaseURL: "https://datn-lora-default-rtdb.firebaseio.com",
    projectId: "datn-lora",
    storageBucket: "datn-lora.appspot.com",
    messagingSenderId: "410661444718",
    appId: "1:410661444718:web:fb9f59f4159a1e0c5cda7f",
    measurementId: "G-VM3G36ES4R"
  };
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();
var database = firebase.database();



/* --------------------------- DECLARE FUNCTIONS ------------------------------- */
function showWarningToast(loc, duration) {
    const main = document.getElementById("toast");
    if (main) {
        const toast = document.createElement("div");
  
        // Auto remove toast
        const autoRemoveId = setTimeout(function () {
            main.removeChild(toast);
        }, duration + 1000);
  
        // Remove toast when clicked
        toast.onclick = function (e) {
            if (e.target.closest(".toast__close")) { // bấm vào X
                main.removeChild(toast);
                clearTimeout(autoRemoveId); // bỏ qua auto remove toast
            }
        };
  
        const delay = (duration / 1000).toFixed(2);
  
        toast.classList.add("toast", `toast--warning`);
        toast.style.animation = `slideInLeft ease .3s, fadeOut linear 1s ${delay}s forwards`;
        toast.innerHTML = `
            <div class="toast__icon">
                <i class="fas fa-exclamation-circle"></i>
            </div>
            <div class="toast__body">
                <h3 class="toast__title">Warning from ${loc}</h3>
                <p class="toast__msg">There is a high risk of fire !!!</p>
            </div>
            <div class="toast__close">
                <i class="fas fa-times"></i>
            </div>
        `;
        main.appendChild(toast);
    }
}

function checkValue(loc, tempVal, flameVal, gasVal) 
{
    if (tempVal >= 100 && flameVal == 0 && gasVal >= 400) {
        showWarningToast(loc, 10000);
        audio.play();
    }
}

function updateValue(loc, flame, gas, humi, temp, boxFlame)
{
    var flameVal, gasVal, tempVal, humiVal;

    database.ref("/" + loc + "/flame").on("value", function(snapshot) {
        if (snapshot.exists()) {
            flameVal = snapshot.val();
            flame.innerHTML = flameVal;
            if (flameVal == 0) {
                boxFlame.classList.remove("boxFlame");
                boxFlame.classList.add("boxFlameWarning");
            }
            else {
                boxFlame.classList.remove("boxFlameWarning");
                boxFlame.classList.add("boxFlame");
            }
            checkValue(loc, tempVal, flameVal, gasVal);
        }    
    });

    database.ref("/" + loc + "/gas").on("value", function(snapshot) {
        if (snapshot.exists()) {
            gasVal = snapshot.val();
            gas.innerHTML = gasVal;
            checkValue(loc, tempVal, flameVal, gasVal);
        }
    });

    database.ref("/" + loc + "/humi").on("value", function(snapshot) {
        if (snapshot.exists()) {
            humiVal = snapshot.val();
            humi.innerHTML = humiVal;
        }
    });

    database.ref("/" + loc + "/temp").on("value", function(snapshot) {
        if (snapshot.exists()) {
            tempVal = snapshot.val();
            temp.innerHTML = tempVal;
            checkValue(loc, tempVal, flameVal, gasVal);
        }
    });
} 

function showTime(time) {
    var d = new Date();
    time.innerHTML = d.toLocaleTimeString();
}



/* --------------------------- VARIABLES ------------------------------- */
var loc1 = "Location 1"
var flame1 = document.getElementById("flame1");
var gas1 = document.getElementById("gas1");
var humi1 = document.getElementById("humi1");
var temp1 = document.getElementById("temp1");
var boxFlame1 = document.getElementById("boxFlame1");

var loc2 = "Location 2"
var flame2 = document.getElementById("flame2");
var gas2 = document.getElementById("gas2");
var humi2 = document.getElementById("humi2");
var temp2 = document.getElementById("temp2");
var boxFlame2 = document.getElementById("boxFlame2");

var loc3 = "Location 3"
var flame3 = document.getElementById("flame3");
var gas3 = document.getElementById("gas3");
var humi3 = document.getElementById("humi3");
var temp3 = document.getElementById("temp3");
var boxFlame3 = document.getElementById("boxFlame3");

var loc4 = "Location 4";
var flame4 = document.getElementById("flame4");
var gas4 = document.getElementById("gas4");
var humi4 = document.getElementById("humi4");
var temp4 = document.getElementById("temp4");
var boxFlame4 = document.getElementById("boxFlame4");

var realtime = document.getElementById("time");

var audio = document.getElementById("alarm");



/* --------------------------- CALL FUNCTIONS ------------------------------- */
updateValue(loc1, flame1, gas1, humi1, temp1, boxFlame1);
updateValue(loc2, flame2, gas2, humi2, temp2, boxFlame2);
updateValue(loc3, flame3, gas3, humi3, temp3, boxFlame3);
updateValue(loc4, flame4, gas4, humi4, temp4, boxFlame4);

setInterval(showTime, 1000, realtime);