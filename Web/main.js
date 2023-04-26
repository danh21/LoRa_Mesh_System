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



function updateValue(loc, flame, gas, humi, temp)
{
    database.ref(loc + "/flame").on("value", function(snapshot) {
        if (snapshot.exists())
            flame.innerHTML = snapshot.val();
    });

    database.ref(loc + "/gas").on("value", function(snapshot) {
        if (snapshot.exists())
            gas.innerHTML = snapshot.val();
    });

    database.ref(loc + "/humi").on("value", function(snapshot) {
        if (snapshot.exists())
            humi.innerHTML = snapshot.val();
    });

    database.ref(loc + "/temp").on("value", function(snapshot) {
        if (snapshot.exists())
            temp.innerHTML = snapshot.val();
    });
} 

function showTime(time) {
    var d = new Date();
    time.innerHTML = d.toLocaleTimeString();
}



var loc1 = "/Location 1"
var flame1 = document.getElementById("flame1")
var gas1 = document.getElementById("gas1")
var humi1 = document.getElementById("humi1")
var temp1 = document.getElementById("temp1")

var loc2 = "/Location 2"
var flame2 = document.getElementById("flame2")
var gas2 = document.getElementById("gas2")
var humi2 = document.getElementById("humi2")
var temp2 = document.getElementById("temp2")

var loc3 = "/Location 3"
var flame3 = document.getElementById("flame3")
var gas3 = document.getElementById("gas3")
var humi3 = document.getElementById("humi3")
var temp3 = document.getElementById("temp3")

var loc4 = "/Location 4"
var flame4 = document.getElementById("flame4")
var gas4 = document.getElementById("gas4")
var humi4 = document.getElementById("humi4")
var temp4 = document.getElementById("temp4")

var realtime = document.getElementById("time")



updateValue(loc1, flame1, gas1, humi1, temp1)
updateValue(loc2, flame2, gas2, humi2, temp2)
updateValue(loc3, flame3, gas3, humi3, temp3)
updateValue(loc4, flame4, gas4, humi4, temp4)

setInterval(showTime, 1000, realtime);