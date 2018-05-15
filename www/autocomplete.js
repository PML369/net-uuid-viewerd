$(document).ready(function() {
	$('#heatmap-prefix').autoComplete({
		minLength: 0,
		events: {
			search : addressPrefixSearch
		}
	});
	$('#heatmap-procname-prefix').autoComplete({
		minLength: 0,
		events: {
			search : procNamePrefixSearch
		}
	});
});

function addressPrefixSearch(query, callback) {
	apiAjax({ command: "address_prefix", prefix: query },
		function(data, textStatus, jqXHR) {
			callback(data);
		});
}

function procNamePrefixSearch(query, callback) {
	apiAjax({ command: "procname_prefix", prefix: query },
		function(data, textStatus, jqXHR) {
			callback(data);
		});
}
