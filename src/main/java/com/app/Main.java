package com.app;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    //fucking cunt
    @Override
    public void start(Stage primaryStage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("../../../resources/views/MainWindow.fxml"));
        primaryStage.setTitle("App");
        primaryStage.setScene(new Scene(root, 600, 300));
        primaryStage.setTitle("Multiplicación de matriceses");
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }

}
