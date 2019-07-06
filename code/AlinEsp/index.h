const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">
<title>Alin web page</title>

<style>

.card{
     max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}

</style>

</head>

<body>

<div class="card">
  <h1>Sensor Value:<span id="encValue">0</span></h1><br>
</div>

<svg width="500" height="1050">
    <rect width="150" height="50" fill="red">
        <animate attributeName="x" from="-150" to="490" dur="1000ms" fill="freeze" repeatCount="1"/> 
    </rect>

    <circle cx="170" cy="170" r="16" stroke="#E2007C"/>

</svg>

<svg width="1000" height="1050">
    <rect width="150" height="50" fill="red">
        <animate attributeName="x" from="-150" to="450"
      dur="1000ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=50 width="150" height="50" fill="orange">
        <animate attributeName="x" from="-150" to="450"
      dur="1200ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=100 width="150" height="50" fill="yellow">
        <animate attributeName="x" from="-150" to="450"
      dur="1400ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=150 width="150" height="50" fill="lightgreen">
        <animate attributeName="x" from="-150" to="450"
      dur="1600ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=200 width="150" height="50" fill="cyan">
        <animate attributeName="x" from="-150" to="450"
      dur="1800ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=250 width="150" height="50" fill="deepskyblue">
        <animate attributeName="x" from="-150" to="450"
      dur="2000ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
    <rect y=300 width="150" height="50" fill="purple">
        <animate attributeName="x" from="-150" to="450"
      dur="2200ms" fill="freeze" repeatCount="indefinite"/> 
    </rect>
</svg>

<script>

setInterval(function() {
  // Call a function repetatively with 0.5 Second interval
  getData();
}, 500); //################ Set calling Time.   -> was 2000  //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("encValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getEncoder", true);
  xhttp.send();
}

</script>

</body>
</html>
)=====";