const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no"/>
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

<svg viewBox = "400 00 600 300">
    <circle class="head" id="head"     cx="500" cy="80"  r="25" stroke="#404041" />
    <rect  class="bPart0" id="body"      x="500"  y="105" width="70" height="10" transform = "rotate(+90  500 110)"
                                                                    fill= "yellow" stroke= "green" stroke-width= "1"/>
    <rect  class="bPart1" id="right_arm" x="500"  y="105" width="70" height="6" transform = "rotate(+30  500 108)" />
    <rect  class="bPart1" id="left_arm"  x="500"  y="105" width="70" height="6" transform = "rotate(+150 500 108)" />
	
    <rect  class="bPart2" id="right_leg" x="500"  y="175" width="100" height="10" > 
        <!--transform = "rotate(+30  500 178)"-->
        <animateTransform attributeName="transform" type="rotate"
        from="85  500 180" to="15  500 180" dur="3s" repeatDur="indefinite" fill="freeze"/>     
    </rect>
    <rect  class="bPart2" id="left_leg"  x="500"  y="175" width="100" height="10" transform = "rotate(+120 500 180)" />
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

var thetaDelta  = 1;
var c           = document.getElementById('left_leg')
var direction   = +1;
var requestAnimationFrameID;
c.currentTheta  = 120; 
c.transform.baseVal.getItem(0).setRotate(c.currentTheta, 500, 180); 
requestAnimationFrameID = requestAnimationFrame(doAnim); // Start the animation loop.

function doAnim()
{
	if (c.currentTheta > 165)
		direction = -1;
	if (c.currentTheta < 95)
		direction = +1;
	  
	c.currentTheta += direction * thetaDelta;
	c.transform.baseVal.getItem(0).setRotate(c.currentTheta, 500, 180); 
	
	requestAnimationFrameID = requestAnimationFrame(doAnim); // continue the animation .
}

</script>

</body>
</html>
)=====";