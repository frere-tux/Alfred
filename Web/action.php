<?php
require_once('common.php');

$result['state']  = 0;
$result['error']  = '';

switch($_['action']){
	case 'changeState':
		//Definis le PIN en tant que sortie
		system("gpio mode ".$pins[$_['pin']]." out");
		//Active/désactive le pin
		system("gpio write ".$pins[$_['pin']]." ".$_['state']);
		$result['state'] = 1;
	break;

	case 'demo':
		system("gpio write ".$pins['11']." 1");
		sleep(1);
		system("gpio write ".$pins['11']." 0");
		system("gpio write ".$pins['12']." 1");
		sleep(1);
		system("gpio write ".$pins['12']." 0");
		system("gpio write ".$pins['7']." 1");
		sleep(1);
		system("gpio write ".$pins['7']." 0");
	break;

	case 'playSound':
		system("sudo aplay match0.wav");
		sleep(1);

	break;

	case 'turnOn':
		system("sudo ../RadioEmission 7 12325261 1 on");
		sleep(1);
	break;

	case 'turnOff':
		system("sudo ../RadioEmission 7 12325261 1 off");
		sleep(1);
	break;

	case 'saveText':
		system("echo ".$_POST['text']." >> myDiary.txt");
		sleep(1);
	break;

	default:
		$result['error']  = 'Aucune action définie';
	break;
}

echo '('.json_encode($result).')';

?>
