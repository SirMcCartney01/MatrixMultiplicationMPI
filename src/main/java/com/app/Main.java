package com.app;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("../../../resources/views/MainWindow.fxml"));
        primaryStage.setTitle("App");
        primaryStage.setScene(new Scene(root, 900, 400));
        primaryStage.setTitle("Multiplicación de matrices");
        primaryStage.show();


        ScrollPane sc = new ScrollPane();
        sc.setFitToHeight(true);
        sc.setFitToWidth(true);

        GridPane grid = new GridPane();
        grid.setAlignment(Pos.TOP_CENTER);
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(5, 5, 5, 5));

        Scene scene = new Scene(grid, 900, 400);
        primaryStage.setScene(scene);

        //Adding contnet
        Text mainTitle = new Text("Multiplicacion de matrices con Cannon MPI");
        mainTitle.setFont(Font.font("Tahoma", FontWeight.BLACK, 40));
        grid.add(mainTitle, 0, 0,2,1);

        Label numCores = new Label("Numero de nucleos\t\t\t\t");
        Label sizeMatrix = new Label("Tamaño de la matriz NxN\t\t");
        numCores.setFont(Font.font("Arial", FontWeight.BLACK, 20));
        sizeMatrix.setFont(Font.font("Arial", FontWeight.BLACK, 20));
        grid.add(numCores, 0, 3);
        grid.add(sizeMatrix, 1, 3);

        Label dataType = new Label("Tipo de dato");
        dataType.setFont(Font.font("Arial", FontWeight.BLACK, 20));
        grid.add(dataType, 2,3);

        //Button
        Button acept = new Button("Aceptar");
        grid.add(acept, 1,9);


        //Radio buttons

        final ToggleGroup CoreGroup = new ToggleGroup();
        final ToggleGroup NGroup = new ToggleGroup();
        final ToggleGroup datType = new ToggleGroup();

        //Radio buttons # of cores
        RadioButton oneCore = new RadioButton("#1");
        oneCore.setToggleGroup(CoreGroup);
        oneCore.setUserData("1");
        oneCore.setSelected(true);
        RadioButton twoCore = new RadioButton("#2");
        twoCore.setToggleGroup(CoreGroup);
        twoCore.setUserData("2");
        RadioButton threeCore = new RadioButton("#3");
        threeCore.setToggleGroup(CoreGroup);
        threeCore.setUserData("3");
        RadioButton fourCore = new RadioButton("#4");
        fourCore.setToggleGroup(CoreGroup);
        fourCore.setUserData("4");
        RadioButton fiveCore = new RadioButton("#5");
        fiveCore.setToggleGroup(CoreGroup);
        fiveCore.setUserData("5");
        RadioButton sixCore = new RadioButton("#6");
        sixCore.setToggleGroup(CoreGroup);
        sixCore.setUserData("6");
        RadioButton sevenCore = new RadioButton("#7");
        sevenCore.setToggleGroup(CoreGroup);
        sevenCore.setUserData("7");
        RadioButton eightCore = new RadioButton("#8");
        eightCore.setToggleGroup(CoreGroup);
        eightCore.setUserData("8");

        //Radio Buttons matrix size
        RadioButton n16 = new RadioButton("16");
        n16.setToggleGroup(NGroup);
        n16.setUserData("16");
        n16.setSelected(true);
        RadioButton n32 = new RadioButton("32");
        n32.setToggleGroup(NGroup);
        n32.setUserData("32");
        RadioButton n64 = new RadioButton("64");
        n64.setToggleGroup(NGroup);
        n64.setUserData("64");
        RadioButton n128 = new RadioButton("128");
        n128.setToggleGroup(NGroup);
        n128.setUserData("64");
        RadioButton n256 = new RadioButton("256");
        n256.setToggleGroup(NGroup);
        n256.setUserData("128");
        RadioButton n512 = new RadioButton("512");
        n512.setToggleGroup(NGroup);
        n512.setUserData("512");
        RadioButton n1024 = new RadioButton("1024");
        n1024.setToggleGroup(NGroup);
        n1024.setUserData("1024");
        RadioButton n2048 = new RadioButton("2048");
        n2048.setToggleGroup(NGroup);
        n2048.setUserData("2048");

        //Radio button Data type
        RadioButton intType = new RadioButton("int");
        intType.setToggleGroup(datType);
        intType.setUserData("int");
        intType.setSelected(true);
        RadioButton floatType = new RadioButton("float");
        floatType.setToggleGroup(datType);
        floatType.setUserData("float");


        //Vertical boxes for radio button groups
        VBox numberCores = new VBox(oneCore, twoCore, threeCore, fourCore, fiveCore, sixCore, sevenCore, eightCore);

        VBox matrixSize = new VBox(n16, n32, n64, n128, n256, n512, n1024, n2048);

        VBox MatrixDataType = new VBox(intType, floatType);

        grid.add(numberCores, 0,5);
        grid.add(matrixSize, 1, 5);
        grid.add(MatrixDataType, 2, 5);


    }

    public static void main(String[] args) {
        launch(args);
    }
}
