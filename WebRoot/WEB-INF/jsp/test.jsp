<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>HelloGoogleMap</title>
	<style>
		#map
		{
			height: 100%;
		}
		  /* Optional: Makes the sample page fill the window. */
		html, body
		{
			height: 100%;
			margin: 0;
			padding: 0;
		}
	</style>
</head>

<body>


<div id="map"></div>
<script>
	function initMap()
	{
		var centerpoint = {lat: 24.36811,lng: 118.0487};
		var cent2 = {lat: 24.3650563046,lng: 118.0471998453};
		var cent3 = {lat: 24.3686235004,lng: 118.0489754677};
		var map = new google.maps.Map(document.getElementById('map'), {
			center: centerpoint,
			zoom: 16
		});

		var maker1= new google.maps.Marker({
			position: cent2,
			map: map
		});
		
		var maker2= new google.maps.Marker({
			position: cent3,
			map: map
		});

	}
</script>

<script 
    async defer
    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyA_UAr_Kpq7BObVW_NgX98F9tpRV2FSejM&callback=initMap">
</script>


</body>
</html>

