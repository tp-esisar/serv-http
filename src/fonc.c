int verif_version (StringL version)
{
	int i = version->len -1;
	int j= 0;
	
	while (version->s[i--] != '.');

	while (version->s[i] != '/')
		nombre += (version->s[i--] - 48)*(j++);
	
	if ( nombre != 1)
		return 1;
	else
		return 0;
	
}

if (verif_version(version))
	reponse = error("505", "Version de HTTP non supportée");
else {
	if (strcmp (methode, "GET") == )
		process_get();
	else if (strcmp (methode, "POST") == )
		prcess_post();
	else	
		reponse = error("501", "V Méthode non supportée");
}
