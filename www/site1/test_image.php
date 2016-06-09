    <html>
    <head>
    <title>Upload</title>
    </head>

    <body>

    <table width="100%" border="0">
      <tr>
        <td>
          <form name="upload" method="post" action="" enctype="multipart/form-data">
            <input type="file" name="fichier_upload" id="fichier_upload"><br>
            <input type="submit" name="Submit" value="Uploader">
          </form>
        </td>
      </tr>
      <tr>
        <td>
    <?php
          if(!empty($_FILES['fichier_upload']['name']))
          {
            // on récupère les infos du fichier à uploader
            $fichier_temp = $_FILES['fichier_upload']['tmp_name'];
            $fichier_nom = $_FILES['fichier_upload']['name'];

            // on défini les dimensions et le type du fichier
            list($fichier_larg, $fichier_haut, $fichier_type, $fichier_attr)=getimagesize($fichier_temp);

            // infos de contrôle du fichier
            $fichier_poids_max = 500000;
            $fichier_h_max = 2448;
            $fichier_l_max = 3264;

            // dossier de destination
            $fichier_dossier = '/home/userir/Desktop/serv-http/www/site1/upload/';
		$fichier_dossier2 = '/upload/';

            // extension du fichier
            $fichier_ext = substr($fichier_nom,strrpos( $fichier_nom, '.')+1);

            // on renomme le fichier
            $fichier_date = date("ymdhis");
            $fichier_n_nom = $fichier_date.".".$fichier_ext;

            // on vérifie s'il y a bien un fichier à uploader
            if (!empty($fichier_temp) && is_uploaded_file($fichier_temp))
            {
              // on vérifie le poids du fichier
              if (filesize($fichier_temp)<$fichier_poids_max)
              {
                // types de fichiers autorises 1=gif / 2=jpg / 3=png
                if (($fichier_type===1) || ($fichier_type===2) || ($fichier_type===3))
                {
                  // on vérifie si l'image n'est pas trop grande
                  if (($fichier_larg<=$fichier_l_max) && ($fichier_haut<=$fichier_h_max))
                  {
                    // si le fichier est ok, on l'upload sur le serveur
                    if (move_uploaded_file($fichier_temp, $fichier_dossier.$fichier_n_nom))
                    {
                      echo "Le fichier a été uploadé avec succès<br />";
                      echo '<a href="'.$fichier_dossier2.$fichier_n_nom.'"><img src="'.$fichier_dossier2.$fichier_n_nom.'"></a><br />';
                    }
                    else
                      echo "Le fichier n'a pas pu être uploadé<br />";
                  }
                  else
                    echo "Le fichier est trop grand<br />";
                }
                else
                  echo "Le fichier n'a pas le bon format<br />";
              }
              else
                echo "Le fichier est trop lourd<br />";
            }
            else
              echo "Pas de fichier à uploader<br />";
          }
    ?>
    	
    	</td>
      </tr>
    </table>
    </body>
    </html>
