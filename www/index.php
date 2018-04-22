<html>
	<head>
		<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
		<script type="text/javascript" src="https://cdn.plot.ly/plotly-latest.min.js"></script>

		<style>
			#dump { width: 1200px; height: 300px; white-space: pre;}
			#heatmap { width: 1200px; border: 2px solid black; }
		</style>
	</head>
	<body>
		<textarea id="dump"></textarea>
		<div id="heatmap"></div>
	</body>
	<script type="text/javascript">
var hexDigits =  ['0', '1', '2', '3', '4', '5', '6', '7',
		  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'];

$(document).ready(function() {
	$('#dump').val("");
	$.ajax({
		url: "/api.php",
		data: {
			command: "dump_data"
		},
		success: function(data, textStatus, jqXHR) {
			$('#dump').val(data);
		},
		error: function(jqXHR, textStatus, errorThrown) {
			if (errorThrown == "Internal Server Error") {
				$('#dump').val(jqXHR.responseText);
			}
		}
	});

	$.ajax({
		url: "/api.php",
		dataType: "json",
		data: {
			command: "address_heatmap",
			prefix: ""
		},
		success: function(data, textStatus, jqXHR) {
			$('#heatmap').css('height', 180+60*data.ydata.length);
			var zeroStr = '0'.repeat(data.numZeros);
			var nineStr = 'f'.repeat(data.numZeros);

			var mapData = [{
				x: hexDigits.map(s => s+zeroStr+
							'<br>-<br>'+
							s+nineStr),
				y: data.ydata.reverse(),
				z: data.zdata.reverse(),
				type: 'heatmap',
				showscale: true,
				colorscale:    [[0  , '#0000ff'],
						[0.5, '#ff00ff'],
						[1  , '#ff0000']]
			} ];
			var layout = {
				xaxis: {
					type: 'category',
					side: 'top',
					automargin: true,
					tickfont: {
						family: 'Courier, Arial'
					}
				},
				yaxis: {
					type: 'category',
					automargin: true,
					tickfont: {
						family: 'Courier, Arial'
					}
				}
			};
			Plotly.plot("heatmap", mapData, layout);
		},
		error: function(jqXHR, textStatus, errorThrown) {
			if (errorThrown == "Internal Server Error") {
				alert(jqXHR.responseText);
			}
		}
	});
});
	</script>
</html>
