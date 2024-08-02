# TPP Physim Example
---

Ejemplo de simulación de varios ESP32 conectados entre sí corriendo sobre QEMU.

## Cómo usarlo

La simulación consiste de varias partes:
- El controlador de la simulación: se encarga de iniciar las instancias de QEMU, "interconectarlas", y administrar los logs.
- El componente de IDF "physim" que se encuentra en este repositorio. Es el encargado de hablar con el controlador.
- El script de configuración (`basic.py`), que se encarga de subir la imagen "flash" al simulador y configurar los enlaces e instancias.

Para iniciar una simulación se deben realizar los siguientes pasos:
1. Buildear la imagen de los ESP32 con `idf.py build`. Observar que el `CMakeLists.txt` de este repositorio tiene un comando especial agregado que genera el archivo `qemu_flash_image.bin` dentro de la carpeta `build`.
2. Iniciar el controlador de simulación con `docker-compose up`. La primera vez que se ejecute este comando va a buildear la imagen de Docker del controlador que puede tomar unos minutos. El controlador está [en otro repo](https://github.com/sportelliluciano/tpp-physim).
3. Configurar y lanzar el simulador corriendo `python basic.py` (o cualquier otro script).
4. Navegar a `http://localhost:13013/logs` para poder ver los logs de los dispositivos. La simulación comienza 5 segundos después de que el script de configuración termine. Los ID de dispositivos van de 0 a N-1, siendo el dispositivo 0 el primero en crearse mediante `qemu_create_instance`.
5. Una vez terminada la simulación se debe cerrar el controlador de simulación dandole un `Ctrl-C` a la terminal de docker-compose (o con `docker-compose stop`). Está planeado a futuro poder resetear el controlador sin tener que cerrarlo pero por ahora eso no está implementado.

