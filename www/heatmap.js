var hexDigits =  ['0', '1', '2', '3', '4', '5', '6', '7',
		  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'];

function genHeatmap() {
	$('#heatmap-load').addClass('fa-refresh fa-spin');
	apiAjax({
			command: "address_heatmap",
			prefix: $('#heatmap-prefix').val(),
			proc_prefix: $('#heatmap-procname-prefix').val()
		}, drawHeatmap);
}

var heatmapAxis = {
	type: 'category',
	automargin: true,
	tickfont: {
		family: 'Courier, Arial'
	}
};

var heatmapLayout = {
	xaxis: {
		prototype: heatmapAxis,
		side: 'top',
	},
	yaxis: heatmapAxis
};
var heatmapData = {
	type: 'heatmap',
	showscale: true,
	hoverinfo: 'z',
	colorscale: [[0  , '#0000ff'], [0.5, '#ff00ff'], [1  , '#ff0000']]
};

function drawHeatmap(data, textStatus, jqXHR) {
	var zeroStr = '0'.repeat(data.numZeros);
	var nineStr = 'f'.repeat(data.numZeros);

	heatmapData.x = hexDigits.map(s => s+zeroStr + '<br>-<br>' + s+nineStr);
	heatmapData.y = data.ydata.reverse();
	heatmapData.z = data.zdata.reverse();
	heatmapLayout.height = 180+60*data.ydata.length;

	Plotly.react("heatmap", [heatmapData], heatmapLayout);
	$('#heatmap-load').removeClass('fa-refresh fa-spin');

	document.getElementById('heatmap').on('plotly_click', heatmapClick);
}

function heatmapClick(data) {
	if (data.points.length > 0 && data.points[0].z > 0) {
		var p = data.points[0];
		$('#heatmap-prefix').val(p.y + p.x[0]);
		genHeatmap();
	}
}
