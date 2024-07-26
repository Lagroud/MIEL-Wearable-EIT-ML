# Documentation

Voici le [lien](https://tvannier1.github.io/MIEL-Wearable-EIT-System-Documentation/) docmumentation officiel du projet.

# Communication Bluetooth 

Un ReadME dédié à l'utilisation de la librairie bluetooth est situé dans le dossier lib du projet. Il faut juste se rendre une fois dans la fenêtre de config sur l'esp32 pour lancer l'instance bluetooth et ensuite lancer le programme python bleak_client.py pour procéder à l'appairage. Ensuite les données capturées par record_cycle seront automatiquement envoyé à votre ordinateur par le biais du script python.
