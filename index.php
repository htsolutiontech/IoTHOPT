<?php
session_start();

// Thông tin kết nối tới MySQL database
$servername = "localhost";
$username = "root";
$password = "H&ptiot2024";
$dbname = "sensor";

// Tạo kết nối
$conn = new mysqli($servername, $username, $password, $dbname);

// Kiểm tra kết nối
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Xử lý đăng ký người dùng
if (isset($_POST['register'])) {
    if (isset($_POST['reg_username']) && isset($_POST['reg_password'])) {
        $reg_username = $conn->real_escape_string($_POST['reg_username']);
        $reg_password = password_hash($conn->real_escape_string($_POST['reg_password']), PASSWORD_BCRYPT);

        $sql = "INSERT INTO users (username, password) VALUES ('$reg_username', '$reg_password')";

        if ($conn->query($sql) === TRUE) {
            echo "Registration successful. <a href='#login'>Login here</a>";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    } else {
        echo "Username and password are required for registration.";
    }
}

// Xử lý đăng nhập người dùng
if (isset($_POST['login'])) {
    if (isset($_POST['login_username']) && isset($_POST['login_password'])) {
        $login_username = $conn->real_escape_string($_POST['login_username']);
        $login_password = $conn->real_escape_string($_POST['login_password']);

        $sql = "SELECT password FROM users WHERE username='$login_username'";
        $result = $conn->query($sql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            if (password_verify($login_password, $row['password'])) {
                $_SESSION['username'] = $login_username;
                header("Location: " . $_SERVER['PHP_SELF']); // Chuyển hướng trang sau khi đăng nhập thành công
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
    header("Location: " . $_SERVER['PHP_SELF']); // Chuyển hướng trang sau khi đăng xuất
    exit();
}

// Xử lý yêu cầu để thêm dữ liệu vào bảng sensor_data
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['temperature']) && isset($_POST['humidityAir']) && isset($_POST['time'])) {
    if (isset($_SESSION['username'])) {
        $temperature = $conn->real_escape_string($_POST['temperature']);
        $humidityAir = $conn->real_escape_string($_POST['humidityAir']);
        $time = $conn->real_escape_string($_POST['time']);
        $username = $conn->real_escape_string($_SESSION['username']);

        // Không nên lưu trữ mật khẩu trực tiếp trong bảng cảm biến
        $password = ''; // Có thể để trống hoặc bỏ qua phần này

        $sql = "INSERT INTO sensor_data (nhiet_do, do_am, created_at, username, password) 
                VALUES ('$temperature', '$humidityAir', '$time', '$username', '$password')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    } else {
        echo "You must be logged in to add data.";
    }
}

// Xử lý yêu cầu AJAX để lấy giá trị mới nhất
if (isset($_GET['latest'])) {
    $sql_latest = "SELECT nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC LIMIT 1";
    $result_latest = $conn->query($sql_latest);

    if ($result_latest->num_rows > 0) {
        $latest_data = $result_latest->fetch_assoc();
        echo json_encode($latest_data);
    } else {
        echo json_encode(["error" => "No data found"]);
    }
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Dashboard</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f7f6;
            color: #333;
            margin: 0;
            padding: 0;
        }

        .container {
            width: 80%;
            margin: 0 auto;
        }

        .auth, .dashboard {
            max-width: 600px;
            margin: 30px auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            text-align: center;
        }

        .auth h3, .dashboard h2 {
            margin-bottom: 20px;
            color: #4CAF50;
        }

        .auth form, .dashboard form {
            margin-bottom: 20px;
        }

        .auth input, .dashboard input {
            width: calc(100% - 20px);
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ddd;
            border-radius: 4px;
        }

        .auth button, .dashboard button {
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            background-color: #4CAF50;
            color: white;
            font-size: 16px;
            margin: 10px 5px;
        }

        .auth button:hover, .dashboard button:hover {
            background-color: #45a049;
        }

        .dashboard .data p {
            font-size: 1.1rem;
            margin: 10px 0;
        }

        table {
            width: 100%;
            margin-top: 20px;
            border-collapse: collapse;
            background-color: #fff;
            border-radius: 8px;
            overflow: hidden;
        }

        table, th, td {
            border: 1px solid #ddd;
            padding: 12px;
            text-align: center;
        }

        th {
            background-color: #4CAF50;
            color: white;
        }

        tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        tr:hover {
            background-color: #ddd;
        }
    </style>
</head>

<body>
    <div class="container">
        <?php if (!isset($_SESSION['username'])): ?>
            <div class="auth">
                <h3 id="login">Login</h3>
                <form method="POST">
                    <input type="text" name="login_username" placeholder="Username" required>
                    <input type="password" name="login_password" placeholder="Password" required>
                    <button type="submit" name="login">Login</button>
                </form>
                <h3>Register</h3>
                <form method="POST">
                    <input type="text" name="reg_username" placeholder="Username" required>
                    <input type="password" name="reg_password" placeholder="Password" required>
                    <button type="submit" name="register">Register</button>
                </form>
            </div>
        <?php else: ?>
            <div class="dashboard">
                <h2>Sensor Dashboard</h2>
                <div class="data">
                    <p><strong>Temperature:</strong> <span id="temperature">Loading...</span> °C</p>
                    <p><strong>Humidity:</strong> <span id="humidity">Loading...</span> %</p>
                    <p><strong>Last updated:</strong> <span id="timestamp">Loading...</span></p>
                </div>
                <form method="POST">
        
                    <button type="submit">Add Data</button>
                    <button type="submit" name="logout">Logout</button>
                </form>
            </div>

            <h2 style="text-align: center;">Data from sensor</h2>
            <table>
                <tr>
                    <th>ID</th>
                    <th>Temperature</th>
                    <th>Humidity</th>
                    <th>Timestamp</th>
                </tr>
                <?php
                $sql_show = "SELECT id, nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC";
                $result = $conn->query($sql_show);

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
                ?>
            </table>
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

        setInterval(fetchLatestData, 5000);

        function timedRefresh(timeoutPeriod) {
            setTimeout(function () {
                location.reload(true);
            }, timeoutPeriod);
        }

        window.onload = function () {
            fetchLatestData();
            timedRefresh(5000);
        };
    </script>
</body>

</html>

<?php
$conn->close();
?>
