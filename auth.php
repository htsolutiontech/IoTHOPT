<?php
session_start();

// Thông tin kết nối tới MySQL database cho user
$userServername = "localhost";
$userUsername = "root";
$userPassword = "H&ptiot2024";
$userDbname = "user"; // Tên database cho user

// Tạo kết nối tới database user
$connUser = new mysqli($userServername, $userUsername, $userPassword, $userDbname);

// Kiểm tra kết nối
if ($connUser->connect_error) {
    die("User database connection failed: " . $connUser->connect_error);
}

// Xử lý đăng ký người dùng
if (isset($_POST['register'])) {
    if (isset($_POST['reg_username']) && isset($_POST['reg_password'])) {
        $reg_username = $connUser->real_escape_string($_POST['reg_username']);
        $reg_password = password_hash($connUser->real_escape_string($_POST['reg_password']), PASSWORD_BCRYPT);

        $sql = "INSERT INTO users (username, password) VALUES ('$reg_username', '$reg_password')";

        if ($connUser->query($sql) === TRUE) {
            echo "Registration successful. <a href='index.php#login'>Login here</a>";
        } else {
            echo "Error: " . $sql . "<br>" . $connUser->error;
        }
    } else {
        echo "Username and password are required for registration.";
    }
}

// Xử lý đăng nhập người dùng
if (isset($_POST['login'])) {
    if (isset($_POST['login_username']) && isset($_POST['login_password'])) {
        $login_username = $connUser->real_escape_string($_POST['login_username']);
        $login_password = $connUser->real_escape_string($_POST['login_password']);

        $sql = "SELECT password FROM users WHERE username='$login_username'";
        $result = $connUser->query($sql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            if (password_verify($login_password, $row['password'])) {
                $_SESSION['username'] = $login_username;
                header("Location: index.php");
                exit();
            } else {
                echo "Invalid password";
            }
        } else {
            echo "Username not found";
        }
    } else {
        echo "Username and password are required for login.";
    }
}

// Xử lý đăng xuất người dùng
if (isset($_POST['logout'])) {
    session_destroy();
    header("Location: index.php");
    exit();
}

$connUser->close();
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User Authentication</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f4;
            color: #333;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
            padding: 0;
        }

        .container {
            background: #ffffff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            max-width: 400px;
            width: 100%;
            text-align: center;
        }

        h2 {
            margin-bottom: 20px;
            color: #333;
        }

        input[type="text"],
        input[type="password"] {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        button {
            background-color: #3498db;
            color: white;
            padding: 12px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            margin: 10px 0;
            width: 100%;
            transition: background-color 0.3s ease;
        }

        button:hover {
            background-color: #2980b9;
        }

        a {
            color: #3498db;
            text-decoration: none;
            display: block;
            margin-top: 10px;
        }

        a:hover {
            text-decoration: underline;
        }
    </style>
</head>

<body>
    <div class="container">
        <!-- Form đăng ký -->
        <h2>Register</h2>
        <form method="post" action="">
            <input type="text" name="reg_username" placeholder="Username" required>
            <input type="password" name="reg_password" placeholder="Password" required>
            <button type="submit" name="register">Register</button>
        </form>

        <!-- Form đăng nhập -->
        <h2 id="login">Login</h2>
        <form method="post" action="">
            <input type="text" name="login_username" placeholder="Username" required>
            <input type="password" name="login_password" placeholder="Password" required>
            <button type="submit" name="login">Login</button>
        </form>

        <!-- Nút đăng xuất -->
        <form method="post" action="">
            <button type="submit" name="logout">Logout</button>
        </form>

        <!-- Liên kết đến đăng nhập sau khi đăng ký thành công -->
        <?php if (isset($_POST['register'])): ?>
            <a href="#login">Login here</a>
        <?php endif; ?>
    </div>
</body>

</html>