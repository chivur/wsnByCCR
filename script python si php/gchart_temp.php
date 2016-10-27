<html>
  <body  background="bkgr.jpg">
   
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
    

    var d = "<?php 
            //echo 3; 
            $dataArray = array();
            $valArray = array();
            $servername = "localhost";
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

            $query = "select * from temperature";

            $result = $conn->query($query);
            $i=1;
            if ($result->num_rows > 0) {
                // output data of each row
                while($row = $result->fetch_assoc()) {
                    //echo $row["Date"] . " "  . $row["Value"] .  "<br>";
                    //array_push($dateArray,$row["Date"]);
                    //array_push($valArray,$row["Value"]);
                    echo $row["Date"];
                    $i = $i + 1;
                }
            } else {
                //echo "0 results";
            }
            
            //echo json_encode($valArray);
            
            $conn->close();
        ?>";
    
  
    var tmp = <?php
            
            $dataArray = array();
            $valArray = array();
            $servername = "localhost";
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

            $query = "select * from temperature";

            $result = $conn->query($query);
            $i=1;
            
            if ($result->num_rows > 0) {
                // output data of each row
                while($row = $result->fetch_assoc()) {
                    //echo $row["Date"] . " "  . $row["Value"] .  "<br>";
                    //array_push($dateArray,$row["Date"]);
                    array_push($valArray,$row["Value"]);
                    $i = $i + 1;
                }
            } else {
                //echo "0 results";
            }
            
            echo json_encode($valArray);
            
            $conn->close();

        ?>;

    var size = <?php
            
            $dataArray = array();
            $valArray = array();
            $servername = "localhost";
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

            $query = "select * from temperature";

            $result = $conn->query($query);
           
            
            echo $result->num_rows;
                
            ?>
    
    google.charts.load('current', {'packages':['line']});
    google.charts.setOnLoadCallback(drawChart);
    
    var my_dates = [];
    var idx = 0;
    var tmp_str="";
    for(i=0; i<size; i++){
        tmp_str = "";
        for(j=i*19;j<(i+1)*19;j++){
            tmp_str+=d[j];
        }
        my_dates.push(tmp_str);
    }
    
    function drawChart(){
      
        var data = new google.visualization.DataTable();
        data.addColumn('string','date time');
        data.addColumn('number','Temperature');
      
        for (i = 0; i < size; i++) { 
            data.addRows([
            [my_dates[i],parseFloat(tmp[i])]
            ]);
        }
        
        var options = {
        chart: {
          title: 'Temperature values',
          subtitle: 'all values recorded',
        },
        curveType: 'function',
        width: 900,
        height: 500
        };
        
      
        var chart = new google.charts.Line(document.getElementById('linechart_material'));
        chart.draw(data, options);
    
    }
        
    
    </script>
    <div id="linechart_material" style="width: 900px; height: 500px; position: absolute;top: 50%; left: 50%;margin: -100px 0 0 -450px;"></div>
  </body>
  
</html>