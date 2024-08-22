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
            echo "Registration successful. <a href='#login'>Login here</a>";
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

// Xử lý thêm dữ liệu cảm biến
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['temperature']) && isset($_POST['humidityAir']) && isset($_POST['time'])) {
    $temperature = $_POST['temperature'];
    $humidityAir = $_POST['humidityAir'];
    $time = $_POST['time'];

    $temperature = $connSensor->real_escape_string($temperature);
    $humidityAir = $connSensor->real_escape_string($humidityAir);
    $time = $connSensor->real_escape_string($time);

    $sql = "INSERT INTO sensor_data (nhiet_do, do_am, created_at) VALUES ('$temperature', '$humidityAir', '$time')";

    if ($connSensor->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $connSensor->error;
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
    <style>
        /* CSS styles here */
    </style>
</head>

<body>
    <div class="container">
        <?php if (!$isLoggedIn): ?>
            <div>
                <h2>Register</h2>
                <form method="post" action="">
                    <input type="text" name="reg_username" placeholder="Username" required>
                    <input type="password" name="reg_password" placeholder="Password" required>
                    <button type="submit" name="register">Register</button>
                </form>

                <h2 id="login">Login</h2>
                <form method="post" action="">
                    <input type="text" name="login_username" placeholder="Username" required>
                    <input type="password" name="login_password" placeholder="Password" required>
                    <button type="submit" name="login">Login</button>
                </form>
            </div>
        <?php else: ?>
            <div class="dashboard">
                <h2>Sensor Dashboard</h2>
                <div>
                    <p><strong>Temperature:</strong> <span id="temperature">Loading...</span> °C</p>
                    <p><strong>Humidity:</strong> <span id="humidity">Loading...</span> %</p>
                    <p><strong>Last updated:</strong> <span id="timestamp">Loading...</span></p>
                </div>
            </div>

            <h2>Data from sensor</h2>
            <table>
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
                                    <td>" . $row['created_at'] . "</td>
                                  </tr>";
                        }
                    } else {
                        echo "<tr><td colspan='4'>No data found in the database.</td></tr>";
                    }

                    $connSensor->close();
                    ?>
                </tbody>
            </table>

            <form method="post" action="">
                <h2>Submit Sensor Data</h2>
                <input type="number" name="temperature" placeholder="Temperature" required>
                <input type="number" name="humidityAir" placeholder="Humidity" required>
                <input type="datetime-local" name="time" placeholder="Time" required>
                <button type="submit">Submit Data</button>
            </form>

            <form method="post" action="">
                <button type="submit" name="logout">Logout</button>
            </form>
        <?php endif; ?>
    </div>

    <script>
        function fetchLatestData() {
            fetch(window.location.href + '?latest=1')
                .then(response => response.json())
                .then(data => {
                    if (!data.error) {
                        document.getElementById('temperature').textContent = data.nhiet_do;
                        document.getElementById('humidity').textContent = data.do_am;
                        document.getElementById('timestamp').textContent = data.created_at;
                    } else {
                        console.error("No data found");
                    }
                })
                .catch(error => console.error('Error fetching data:', error));
        }

        function timedRefresh(timeoutPeriod) {
            setTimeout(function () {
                location.reload(true);
            }, timeoutPeriod);
        }

        window.onload = function () {
            if (document.getElementById('temperature')) {
                fetchLatestData();
                timedRefresh(5000); // Refresh every 5 seconds
            }
        }
    </script>
</body>

</html>