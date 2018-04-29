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
	apiAjax({ command: "layer_timings", layers: "[\"IP\"]" },
		function(data, textStatus, jqXHR) {
			alert(data);
		});
}
