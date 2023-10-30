package agricultureapp;

import agricultureapp.DataBase.ApiClient;

public class App {
    
    public static void main(String[] args) {
        
        ApiClient apiClient = new ApiClient();
        
        // Consultar datos
        apiClient.fetchAndPrintTecnicaData();

        // Crear datos
        apiClient.createTecnica();

        

    }
}
