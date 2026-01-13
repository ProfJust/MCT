""" Fernbedienung / Teleoperation für Alvik-WASD über Bluetooth LE.
    Steuerung über Tastatur:
      W/A/S/D  fahren
      Q/E      sanfte Kurve
      SPACE    Stop     
      X        Stop
      +/-      Speed    
      Ctrl+C   Ende

zugehörige CPP- Software auf dem Alvik: WASD_Steuerung_Arduino_Alvik_per_Bluetooth.ino
"""

import asyncio
import time
import msvcrt

from bleak import BleakClient, BleakScanner

DEVICE_NAME = "Alvik-WASD"
UART_RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

VALID = set("WASDQEX+-")
STOP_KEYS = {'X', ' '}   # X oder SPACE stoppen
POLL_DELAY = 0.02        # 20 ms


async def main():
    print("Suche BLE-Gerät ...")
    dev = await BleakScanner.find_device_by_filter(
        lambda d, ad: (d.name or "") == DEVICE_NAME
    )
    if not dev:
        print("❌ Alvik-WASD nicht gefunden")
        return

    print(f"Verbinde zu {dev.address} ({dev.name}) ...")
    async with BleakClient(dev) as client:
        print("✅ Verbunden")
        print("Steuerung:")
        print("  W/A/S/D  fahren")
        print("  Q/E      sanfte Kurve")
        print("  SPACE    Stop")
        print("  X        Stop")
        print("  +/-      Speed")
        print("  Ctrl+C   Ende\n")

        last_cmd = None

        while True:
            if msvcrt.kbhit():
                ch = msvcrt.getch()

                # Ctrl+C
                if ch == b'\x03':
                    print("\nBeendet (Ctrl+C)")
                    await client.write_gatt_char(UART_RX_UUID, b'X', response=False)
                    break

                try:
                    c = ch.decode("ascii")
                except UnicodeDecodeError:
                    continue

                # Space → Stop
                if c == ' ':
                    if last_cmd != 'X':
                        await client.write_gatt_char(
                            UART_RX_UUID, b'X', response=False
                        )
                        print("→ STOP (SPACE)")
                        last_cmd = 'X'
                    continue

                c = c.upper()

                if c in VALID and c != last_cmd:
                    await client.write_gatt_char(
                        UART_RX_UUID,
                        c.encode("ascii"),
                        response=False
                    )
                    print(f"→ {c}")
                    last_cmd = c

            await asyncio.sleep(POLL_DELAY)


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nAbbruch")
