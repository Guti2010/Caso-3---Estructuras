package agricultureapp;

import agricultureapp.DataBase.ApiClient;

public class App {
    
    public static void main(String[] args) {
        
        ApiClient apiClient = new ApiClient();
        apiClient.fetchAndPrintTecnicaData();

        

    }
}
