<html>
	<head>
		<title>PHP Test</title>
	</head>
	<body>
		<?php 
			echo '<p>Hello World</p>'; 
			echo htmlspecialchars($_GET["name"]);
		?> 
	</body>
</html>
