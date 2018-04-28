function apiAjax(data, successFn, dataType="json") {
	$.ajax({
		url: "/api.php",
		data: data,
		dataType: dataType,
		success: successFn,
		error: function(jqXHR, textStatus, errorThrown) {
			if (errorThrown == "Internal Server Error") {
				$('#dump').val(jqXHR.responseText);
			}
		}
	});
}
