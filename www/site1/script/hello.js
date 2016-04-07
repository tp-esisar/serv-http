var temps = 0;
var compteur;
function lancerCompteur(tempsInitial)
		{
		temps = tempsInitial
		actualiserCompteur();
		compteur = setInterval("actualiserCompteur()",1000);
		}
function actualiserCompteur()
		{
		if (temps < 0)
				{
				alert("J'explose !");
				clearInterval(compteur);
				}

		temps = temps;
		var div = document.getElementById("temps");
		var texte = document.createTextNode("Dans "+ temps + " secondes je vais exploser !");
		
		if (div.firstChild != null)
				{
				div.removeChild(div.firstChild);
				}
		div.appendChild(texte);
		temps --;
		
		}