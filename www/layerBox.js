$(document).ready(function() {
	apiAjax({ command: "defined_layers" },
		function(data, textStatus, jqXHR) {
			var layers = $('#layers');
			var layerTemplate = $('#templates > #layer');
			layers.html("");
			layers.append(layerTemplate.clone().html(""));
			data.forEach(function (l) {
				var layer = layerTemplate.clone();
				layer.attr('id', '');
				layer.find('input').attr('id', 'layer-'+l);
				layer.find('label').attr('for', 'layer-'+l)
					.html(l);
				layers.append(layer);
			});
			layers.append(layerTemplate.clone().html(""));
		});
});

function genLayerBox() {
	var layers = [];
	$('#layers label').each(function() {
		var lb = $(this);
		if (document.getElementById(lb.attr('for')).checked)
			layers.push(lb.text());
	});

	apiAjax({ command: "layer_timings", layers: JSON.stringify(layers) },
		function(data, textStatus, jqXHR) {
			var boxData = [];
			data.forEach(function(l) {
				boxData.push({
					x: l.data,
					type: 'box',
					name: l.layer + ' Layer'
				});
			});
			Plotly.react('layer-boxplot', boxData, {
					height: 150*boxData.length,
					xaxis: {
						title: 'Transit time / \u00B5s'
					}
				});
		});
}
