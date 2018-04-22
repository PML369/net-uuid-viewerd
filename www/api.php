<?php
if (!isset($_GET['command'])) {
	http_response_code(400);
	exit;
}

$cmds = array(	"dump_data" => array(),
		"address_heatmap" => array("prefix" => false));

$cmd = $_GET['command'];
if (!isset($cmds[$cmd])) {
	http_response_code(400);
	exit;
}

$request = $cmd." ";
foreach ($cmds[$cmd] as $param => $hasSpaces) {
	if (!isset($_GET[$param])) {
		http_response_code(400);
		exit;
	}
	$request .= $_GET['param'] . ($hasSpaces ? '\0' : ' ');
}

$request = strlen($request) . " $request";

error_reporting(error_reporting() ^ E_WARNING);
$sock = stream_socket_client('unix:///tmp/net-uuid.sock', $errno, $errstr, 20);
error_reporting(error_reporting() | E_WARNING);

if (!$sock) {
	http_response_code(500);
	echo "Socket error connecting to net-uuid-viewerd. ".
		"Error $errno: $errstr";
	exit;
}
fwrite($sock, $request);
$length = -1;
$str = "";
while (!feof($sock)) {
	$str .= fgets($sock);
	if ($length === -1) {
		$arr = explode(' ', $str, 2);
		$length = intval($arr[0]);
		$str = $arr[1];
	}
	if (strlen($str) > $length) {
		break;
	}
}
if (strlen($str) !== $length) {
	http_response_code(500);
	echo "Error: Wrong amount of data received from socket.";
	echo "Received ".strlen($str).", expected $length";
	exit;
}
fclose($sock);
echo $str;
?>
