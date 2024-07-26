/**
 * @file BLE_CONFIG_H
 * @brief Définition des UUIDs pour le service BLE et les caractéristiques de transmission et de réception.
 */

#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

/**
 * @def UUID_SERVICE
 * @brief UUID du service BLE.
 */
#define UUID_SERVICE "abc85f60-08b1-4c93-8f86-2d6c43af7bcf"

/**
 * @def UUID_TX
 * @brief UUID de la caractéristique de transmission (Tx).
 */
#define UUID_TX "69a16f3a-3e2d-4ad3-ab19-2ef634d305b1"

/**
 * @def UUID_RX
 * @brief UUID de la caractéristique de réception (Rx).
 */
#define UUID_RX "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#endif // BLE_CONFIG_H
