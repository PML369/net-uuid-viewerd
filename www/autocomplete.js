$(document).ready(function() {
	$('#heatmap-prefix').autoComplete({
		events: {
			search : addressPrefixSearch
		}
	});
});

function addressPrefixSearch(query, callback) {
	apiAjax({ command: "address_prefix", prefix: query },
		function(data, textStatus, jqXHR) {
			callback(data);
		});
}
