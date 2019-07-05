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