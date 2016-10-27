<html>
<body background="bkgr.jpg">

<style>

p{
    color: white;
    font-size: xx-large;
    margin-left: 20cm;
}

#titlu{
    margin-top: 10cm;
    margin-left: 23cm;
}

#val{
    margin-top: 2cm;
}

#form1{
    margin-top: 2cm;
}

#form2{
    margin-top: 1cm;
}

#form3{
    margin-top: 1cm;
}

input{
    font-size: large;
}

</style>

<p id="titlu" >Valori : </p>

<p id="val">
    Temperature = <br>
    Pressure = <br>
    Motion : <br>
    Humidity = <br>
    Gas level : <br>
</p>

<p id="form1" align="center">
    <form align="center" method="post" action="gchart_temp.php">
        <input type="submit" name="temp" value="Temperature Graph"> 
</form>
</p>


<p id="form2" align="center">
    <form align="center" method="post" action="gchart_humid.php">
        <input type="submit" name="humid" value="Humidity Graph"> 
</form>
</p>


<p id="form3" align="center">
    <form align="center" method="post" action="gchart_pressure.php">
        <input type="submit" name="pressure" value="Pressure Graph"> 
</form>
</p>



<script>

var s=1;

function updt(){
	
	document.getElementById("val").innerHTML = "<?php
	
	
	$servername = "127.0.0.1";
	$username = "user";
	$password = "psswd";
	$dbname = "wordpress";

	// Create connection
	$conn = new mysqli($servername, $username, $password,$dbname);


	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 
	//echo "Connected successfully";

	$query = "select * from latest";

	$result = $conn->query($query);
	$i=0;
    $names_arr = array("Temperature ","Pressure ","Motion ","Humidity ","Gas level ");
    
	if ($result->num_rows > 0) {
		// output data of each row
		while($row = $result->fetch_assoc()) {
            if ($names_arr[$i] == "Temperature "){
                echo $names_arr[$i] . "= "  . $row["Value"] . "\u00B0C" . " @ " . $row["Date"] .  "<br>";
            }
            if ($names_arr[$i] == "Pressure "){
                echo $names_arr[$i] . "= "  . $row["Value"] . "mmHg" . " @ " . $row["Date"] . "<br>";
            }
            if ($names_arr[$i] == "Humidity "){
                echo $names_arr[$i] . "= "  . $row["Value"] . "%" . " @ " . $row["Date"] .  "<br>";
            }
            if ($names_arr[$i] == "Motion "){
                echo $names_arr[$i] . " : "  . $row["Extras"] . " @ " . $row["Date"] .  "<br>";
            }
			if ($names_arr[$i] == "Gas level "){
                echo $names_arr[$i] . " : "  . $row["Extras"] . " @ " . $row["Date"] .  "<br>";
            }
			$i = $i + 1;
		}
	} else {
		echo "0 results";
	}

	$conn->close();

?>"
	
	s+=1;   
}
setInterval(updt,1000);

function f(){
	location.reload();
}
setInterval(f,3000);

</script>
</body>
</html>
