<!DOCTYPE html>
<html>
<style>
    .logout-button {
        position: fixed;
        bottom: 20px;
        left: 50%;
        transform: translateX(-50%);
    }
</style>

<button class="back-button" onclick="window.location.href = 'index.php'">Retour au menu</button>


<button class="logout-button" onclick="window.location.href = 'logout.php'">se deconnecter</button>

<head>
    <title>Profil Utilisateur</title>
</head>
<body>
    <h1>Profil Utilisateur</h1>
    <table>
        <tr>
        </tr>
        <?php
        session_start();
        $servername = "localhost";
        $username = "root";
        $password = "";
        $dbname = "login";

        $conn = new mysqli($servername, $username, $password, $dbname);

        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }
        $email = $_SESSION['email'];
        $sql = "SELECT * FROM utilisateur WHERE email = '$email'";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            while($row = $result->fetch_assoc()) {
                echo "<tr>";
                echo "<td>Email: " . $row["email"] . "</td>";
                echo "</tr>";
                echo "<tr>";
                echo "<td>Nom et Prenom: " . $row["nom_prenom"] . "</td>";
                echo "</tr>";
                echo "<tr>";
                echo "<td>Carte Etudiante: " . $row["carte_etudiante"] . "</td>";
                echo "</tr>";
                echo "<tr>";
                echo "<td>Mot de Passe: " . $row["mot_de_passe"] . "</td>";
                echo "</tr>";
                echo "<tr>";
                echo "<td>Adresse: " . $row["adresse"] . "</td>";
                echo "</tr>";
                echo "<tr>";
                echo "<td><button onclick=\"togglePaymentMethod(this)\">Show Payment Method</button></td>";
                echo "</tr>";
                echo "<tr id=\"paymentMethodRow\" style=\"display: none;\">";
                echo "<td>Moyen de Paiement: " . $row["moyen_de_paiement"] . "</td>";
                echo "</tr>";
                echo "<tr>";
            }
        } else {
            echo "<tr><td colspan='7'>No user found</td></tr>";
        }

        $conn->close();
        ?>
    </table>
    <script>
        function togglePaymentMethod(button) {
            var paymentMethodRow = document.getElementById("paymentMethodRow");
            if (paymentMethodRow.style.display === "none") {
                paymentMethodRow.style.display = "table-row";
                button.textContent = "Hide Payment Method";
            } else {
                paymentMethodRow.style.display = "none";
                button.textContent = "Show Payment Method";
            }
        }
    </script>
