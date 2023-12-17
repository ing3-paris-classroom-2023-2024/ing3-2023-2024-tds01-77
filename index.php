<!DOCTYPE html>
<html>
<head>
    <title>Hub Page</title>
    <meta charset="utf-8">
</head>
<body>
    <h1>Welcome to the Hub Page</h1>
    <?php
        session_start();
        $sessionEmail = isset($_SESSION['email']) ? $_SESSION['email'] : '';
    ?>
    <nav>
        <ul>
            <li><a id="profileLink" href="login.html">Connect</a></li>
        </ul>
    </nav>

    <script>
        var sessionEmail = "<?php echo $sessionEmail; ?>";
        
        if (sessionEmail === "") {
            document.getElementById('profileLink').textContent = 'Connect';
            document.getElementById('profileLink').href = 'login.html';
        } else {
            document.getElementById('profileLink').textContent = sessionEmail;
            document.getElementById('profileLink').href = 'profil.php';
        }
    </script>
</body>
</html>
