<?php
require_once('configuration.php');
require_once('functions.php');
?>

<!doctype html>
<!--[if lt IE 7]> <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang="en"> <![endif]-->
<!--[if IE 7]>    <html class="no-js lt-ie9 lt-ie8" lang="en"> <![endif]-->
<!--[if IE 8]>    <html class="no-js lt-ie9" lang="en"> <![endif]-->
<!--[if gt IE 8]><!--> <html class="no-js" lang="en"> <!--<![endif]-->
<head>
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	<title>Bat Ordinateur</title>
	<meta name="description" content="">
	<meta name="author" content="">
	<meta name="viewport" content="width=device-width">
	<link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div id="header">
		<h1>A.L.F.R.E.D. v0.2</h1>
	</div>

    <!-- Tab links -->
    <div id="mainTab" class="tab">
        <button class="tablinks" onclick="openTab(event, 'objects')" id="defaultOpen">Objets</button>
        <button class="tablinks" onclick="openTab(event, 'tasks')">Taches</button>
        <button class="tablinks" onclick="openTab(event, 'notes')">Notes</button>
    </div>

    <!-- Tab content -->
    <div id="objects" class="tabcontent" style="clear:left">
        <?php
        $json = json_decode(file_get_contents("/home/pi/Projects/Alfred/Data/Objects.dat"), true);

        foreach ($json["rooms"] as $room)
        {
            echo "<div>";
            $roomId = $room["id"];
            $roomName = $room["name"];
            $objectGroupId = $room["groupId"];
            echo "<h2>$roomName</h2>";

            foreach ($room["objects"] as $object)
            {
                $objectId = $object["id"];
                $objectName = $object["name"];
                $objectElementId = $object["elementId"];

                echo "<p>$objectName</br>";
                echo '<button onclick="sendSimpleRequest(', $roomId, ', ', $objectId, ', 1, 0);" class="button buttonGreen" type="button">ON</button>';
                echo '<button onclick="sendSimpleRequest(', $roomId, ', ', $objectId, ', 0, 0);" class="button buttonRed" type="button">OFF</button></p>';
            }

            if (count($room["objects"]) > 1)
            {
                echo "<p><b>Tous</b></br>";
                echo '<button onclick="sendSimpleRequest(', $roomId, ', 0, 1, 1);" class="button buttonGreen" type="button">ON</button>';
                echo '<button onclick="sendSimpleRequest(', $roomId, ', 0, 0, 1);" class="button buttonRed" type="button">OFF</button></p>';
            }

            echo "</br></div>";
        }
        ?>
    </div>

    <div id="tasks" class="tabcontent" style="clear:left">
        <h2>Nouvelle tache</h2>

		<p>Début</br>
		<input type="date" id="taskStartDate" value="<?php echo date("Y-m-d");?>"><input type="time" id="taskStartTime" value="<?php echo date("H:i");?>"></p>
		<p>Fin</br>
		<input type="date" id="taskEndDate" value="<?php echo date("Y-m-d");?>"><input type="time" id="taskEndTime" value="<?php echo date("H:i");?>"></p>
		<p>
            Périodicité</br>
            <input type="number" id="taskPeriodicity" value="0">
            <select id="taskPeriodicityUnit">
                <option value="seconds">Seconds</option>
                <option value="minutes">Minutes</option>
                <option value="hours">Heures</option>
                <option value="days">Jours</option>
            </select>
        </p>
		<p>
            Durée</br>
            <input type="number" id="taskDuration" value="1">
            <select id="taskDurationUnit">
                <option value="seconds">Seconds</option>
                <option value="minutes">Minutes</option>
                <option value="hours">Heures</option>
                <option value="days">Jours</option>
            </select>
        </p>

        <div id="startRequests" class="insert">
            <p>Requêtes de début <button onclick="addElement('startRequestFormTemplate', 'startRequests');" class="smallButton" type="button">+</button> </p>
            <p id="startRequestFormTemplate" style="display:none">
                <label>Object</label>
                <select class="requestObject" style="width:100px">
                    <?php
                        foreach ($json["rooms"] as $room)
                        {
                            echo '<option value=', $room["id"], ':-1>', $room["name"], '</option>';

                            foreach ($room["objects"] as $object)
                            {
                                echo '<option value=', $room["id"], ':', $object["id"], '>--', $object["name"], '</option>';
                            }
                        }
                    ?>
                </select>
                <label>Etat</label> <input class="requestState" type="checkbox">
                <button onclick="removeElement(this);" class="smallButton" type="button">-</button>
            </p>
        </div>
        </br>
        <div id="endRequests" class="insert">
            <p>Requêtes de fin <button onclick="addElement('endRequestFormTemplate', 'endRequests');" class="smallButton" type="button">+</button> </p>
            <p id="endRequestFormTemplate" style="display:none">
                <label>Object</label>
                <select class="requestObject" style="width:100px">
                    <?php
                        foreach ($json["rooms"] as $room)
                        {
                            echo '<option value=', $room["id"], ':-1>', $room["name"], '</option>';

                            foreach ($room["objects"] as $object)
                            {
                                echo '<option value=', $room["id"], ':', $object["id"], '>--', $object["name"], '</option>';
                            }
                        }
                    ?>
                </select>
                <label>Etat</label> <input class="requestState" type="checkbox">
                <button onclick="removeElement(this);" class="smallButton" type="button">-</button>
            </p>
        </div>

        </br>
        <button onclick="sendTask();" class="button buttonOrange" type="button">Enregistrer</button>
    </div>

    <div id="notes" class="tabcontent" style="clear:left">
        <p>Something on your mind?</p>
		<p><input type="text" id="inputToSave" /></p>
		<button onclick="saveText();" class="button buttonOrange" type="button">Save it!</button>
    </div>

    <div id="info" style="clear:left">
		<p id="infoContent"></p>
	</div>

    <script src="js/jquery.min.js"></script>
    <script src="js/script.js"></script>
    <script>
        document.getElementById("defaultOpen").click();

        function openTab(evt, tabName)
        {
            var i, tabcontent, tablinks;
            tabcontent = document.getElementsByClassName("tabcontent");
            for (i = 0; i < tabcontent.length; i++) {
                tabcontent[i].style.display = "none";
            }
            tablinks = document.getElementsByClassName("tablinks");
            for (i = 0; i < tablinks.length; i++) {
                tablinks[i].className = tablinks[i].className.replace(" active", "");
            }
            document.getElementById(tabName).style.display = "block";
            evt.currentTarget.className += " active";
        }
    </script>

</body>
</html>
