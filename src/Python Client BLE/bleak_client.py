import asyncio
from bleak import BleakClient

BLE_ADDRESS = "F4:12:FA:84:5C:19"
CHARACTERISTIC_UUID_NOTIFY = "69a16f3a-3e2d-4ad3-ab19-2ef634d305b1"
FILENAME = "SensorData.csv"


# Vérifie si le fichier existe déjà
try:
    with open(FILENAME, "r") as file:
        print("File already exists, will append to it")
except FileNotFoundError:
    print("File does not exist, creating it")
    with open(FILENAME, "w") as file:
        # Écrit les en-têtes des colonnes dans le fichier CSV
        for i in range(0, 8):
            for j in range(i + 1, 8):
                file.write(f"Electrode {i}-{j};")
        file.write("Gesture\n")

async def notification_handler(sender, data):
    """
    Gère les notifications reçues du périphérique BLE.

    :param sender: Identifiant de l'expéditeur (caractéristique)
    :param data: Données reçues du périphérique BLE
    """
    try:
        decoded_data = data.decode('utf-8')
        if decoded_data == "<END>":
            print("End of CSV data received")
        else:
            print(f"Received: {decoded_data}")
            with open(FILENAME, "a") as file:
                file.write(decoded_data)
    except UnicodeDecodeError:
        print(f"Received non-UTF-8 data: {data}")

async def run(client):
    """
    Lance le client BLE et s'abonne aux notifications.

    :param client: Instance de BleakClient pour gérer la connexion BLE
    """
    await client.start_notify(CHARACTERISTIC_UUID_NOTIFY, notification_handler)
    print("Subscribed to notifications")

    # Boucle pour maintenir la connexion et continuer à recevoir des notifications
    while True:
        await asyncio.sleep(1.0)

async def main():
    """
    Fonction principale pour gérer la connexion BLE avec le client.
    """
    async with BleakClient(BLE_ADDRESS) as client:
        await run(client)

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Program interrupted by user")
