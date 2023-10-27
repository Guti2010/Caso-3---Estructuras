package agricultureapp.DataBase;

import com.contentful.java.cda.CDAClient;
import com.contentful.java.cda.CDAEntry;
import com.contentful.java.cda.LocalizedResource;

public class ApiClient {
    private CDAClient client;

    public ApiClient() {
        client = CDAClient.builder()
                .setSpace("i2qqvp4vv9gi")
                .setToken("mlkomY8HSxHYPd4MZAI5t2x-DgCi9VCZ1BPT-HYe8lE")
                .build();
    }

    public void fetchAndPrintTecnicaData() {
        CDAEntry entry = client
                .fetch(CDAEntry.class)
                .one("6b5ukv3wfb9bDn1VZHPywG");

        if (entry != null) {
            String nombre = entry.getField("nombre");
            String descripcion = entry.getField("descripcion");
            Double costoEstimado = entry.getField("costoEstimado");

            System.out.println("Nombre: " + nombre);
            System.out.println("Descripción: " + descripcion);
            System.out.println("Costo Estimado: " + costoEstimado);
        } else {
            System.out.println("Entrada no encontrada.");
        }
    }

    
}