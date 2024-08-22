<?php
session_start();

// Thông tin kết nối tới MySQL database cho người dùng
$userServername = "localhost";
$userUsername = "root";
$userPassword = "H&ptiot2024";
$userDbname = "user";

// Thông tin kết nối tới MySQL database cho cảm biến
$sensorServername = "localhost";
$sensorUsername = "root";
$sensorPassword = "H&ptiot2024";
$sensorDbname = "sensor";

// Tạo kết nối tới database người dùng
$connUser = new mysqli($userServername, $userUsername, $userPassword, $userDbname);

// Tạo kết nối tới database cảm biến
$connSensor = new mysqli($sensorServername, $sensorUsername, $sensorPassword, $sensorDbname);

// Kiểm tra kết nối
if ($connUser->connect_error) {
    die("User database connection failed: " . $connUser->connect_error);
}
if ($connSensor->connect_error) {
    die("Sensor database connection failed: " . $connSensor->connect_error);
}

// Xử lý đăng ký người dùng
if (isset($_POST['register'])) {
    if (isset($_POST['reg_username']) && isset($_POST['reg_password'])) {
        $reg_username = $connUser->real_escape_string($_POST['reg_username']);
        $reg_password = password_hash($connUser->real_escape_string($_POST['reg_password']), PASSWORD_BCRYPT);

        $sql = "INSERT INTO users (username, password) VALUES ('$reg_username', '$reg_password')";

        if ($connUser->query($sql) === TRUE) {
            echo "<div class='alert alert-success'>Registration successful. <a href='#login'>Login here</a></div>";
        } else {
            echo "<div class='alert alert-danger'>Error: " . $sql . "<br>" . $connUser->error . "</div>";
        }
    } else {
        echo "<div class='alert alert-warning'>Username and password are required for registration.</div>";
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
                echo "<div class='alert alert-danger'>Invalid password</div>";
            }
        } else {
            echo "<div class='alert alert-danger'>Username not found</div>";
        }
    } else {
        echo "<div class='alert alert-warning'>Username and password are required for login.</div>";
    }
}

// Xử lý đăng xuất người dùng
if (isset($_POST['logout'])) {
    session_destroy();
    header("Location: index.php");
    exit();
}

// Xử lý thêm dữ liệu cảm biến
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['temperature']) && isset($_POST['humidity']) && isset($_POST['time'])) {
    $temperature = $connSensor->real_escape_string($_POST['temperature']);
    $humidity = $connSensor->real_escape_string($_POST['humidity']);
    $time = $connSensor->real_escape_string($_POST['time']);

    $sql = "INSERT INTO sensor_data (nhiet_do, do_am, created_at) VALUES ('$temperature', '$humidity', '$time')";

    if ($connSensor->query($sql) === TRUE) {
        echo "<div class='alert alert-success'>New record created successfully</div>";
    } else {
        echo "<div class='alert alert-danger'>Error: " . $sql . "<br>" . $connSensor->error . "</div>";
    }
}

// Xử lý yêu cầu dữ liệu mới nhất
if (isset($_GET['latest'])) {
    $sql_latest = "SELECT nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC LIMIT 1";
    $result_latest = $connSensor->query($sql_latest);

    if ($result_latest->num_rows > 0) {
        $latest_data = $result_latest->fetch_assoc();
        echo json_encode($latest_data);
    } else {
        echo json_encode(["error" => "No data found"]);
    }
    exit();
}


// if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['updateTable'])) {
//     $sql_show = "SELECT id, nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC";
//     $result = $connSensor->query($sql_show);

//     if ($result->num_rows > 0) {
//         while ($row = $result->fetch_assoc()) {
//             echo "<tr>
//                     <td>" . $row['id'] . "</td>
//                     <td>" . $row['nhiet_do'] . "</td>
//                     <td>" . $row['do_am'] . "</td>
//                     <td>" . $row['created_at'] . "</td>
//                   </tr>";
//         }
//     } else {
//         echo "<tr><td colspan='4'>No data found in the database.</td></tr>";
//     }
//     exit();
// }

// Đóng kết nối
$connUser->close();
$connSensor->close();

// Kiểm tra xem người dùng đã đăng nhập chưa
$isLoggedIn = isset($_SESSION['username']);
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Dashboard</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="Backend/styles.css" rel="stylesheet">
</head>

<body>
    <img src="Backend/XENON.jpg" alt="">
    <div class="container">
        <?php if (!$isLoggedIn): ?>
            <div class="card">
                <div class="card-header">
                    Register
                </div>
                <div class="card-body">
                    <form method="post" action="">
                        <div class="mb-3">
                            <input type="text" name="reg_username" class="form-control" placeholder="Username" required>
                        </div>
                        <div class="mb-3">
                            <input type="password" name="reg_password" class="form-control" placeholder="Password" required>
                        </div>
                        <button type="submit" name="register" class="btn btn-primary">Register</button>
                    </form>
                    <hr>
                    <h5 id="login">Login</h5>
                    <form method="post" action="">
                        <div class="mb-3">
                            <input type="text" name="login_username" class="form-control" placeholder="Username" required>
                        </div>
                        <div class="mb-3">
                            <input type="password" name="login_password" class="form-control" placeholder="Password"
                                required>
                        </div>
                        <button type="submit" name="login" class="btn btn-primary">Login</button>
                    </form>
                </div>
            </div>
        <?php else: ?>

            <div class="card">
                <div class="card-header">
                    Sensor Dashboard
                </div>
                <div class="card-body">
                    <div class="mb-3">
                        <p><strong>Temperature:</strong> <span id="temperature">Loading...</span> °C</p>
                        <p><strong>Humidity:</strong> <span id="humidity">Loading...</span> %</p>
                        <p><strong>Last updated:</strong> <span id="timestamp">Loading...</span></p>
                    </div>
                </div>
            </div>

            <div class="card">
                <div class="card-header">
                    Data from Sensor
                </div>
                <div class="card-body">
                    <table class="table table-striped">
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Temperature</th>
                                <th>Humidity</th>
                                <th>Timestamp</th>
                            </tr>
                        </thead>
                        <tbody>
                            <?php
                            // Kết nối lại đến database cảm biến
                            $connSensor = new mysqli($sensorServername, $sensorUsername, $sensorPassword, $sensorDbname);

                            $sql_show = "SELECT id, nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC";
                            $result = $connSensor->query($sql_show);

                            if ($result->num_rows > 0) {
                                while ($row = $result->fetch_assoc()) {
                                    echo "<tr>
                                            <td>" . $row['id'] . "</td>
                                            <td>" . $row['nhiet_do'] . "</td>
                                            <td>" . $row['do_am'] . "</td>
                                            <td>" . $row['created_at'] . "</td>";
                                }
                            } else {
                                echo "<tr><td colspan='4'>No data found in the database.</td></tr>";
                            }

                            $connSensor->close();
                            ?>
                        </tbody>
                    </table>
                </div>
            </div>

            <form method="post" action="" class="mt-4">
                <button type="submit" name="logout" class="btn btn-danger">Logout</button>
            </form>
        <?php endif; ?>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
    <script src="Backend/script.js"></script>
</body>

</html>