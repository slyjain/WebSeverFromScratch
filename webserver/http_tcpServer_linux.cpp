#include<http_tcpServer_linux.h>
#include<iostream>
#include<sstream>
#include<unistd.h>

namespace{ 
    void log(const std::string &message){
        std::cout<<message<<std::endl;
    }
    void exit_with_error(const std::string &errorMessage){
        log("ERROR: "+errorMessage);
        exit(1);
    }
}

namespace http{
    int TcpServer::startServer() {
    // Step 1: Create socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        exit_with_error("Cannot create socket");
        return 1;
    }

    // Step 2: Configure socket address
    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    // Step 3: Bind socket to IP and port
    if (bind(m_socket, (sockaddr*)&m_socketAddress, m_socketAddress_len) < 0) {
        exit_with_error("Cannot bind socket to address");
        return 1;
    }

    // Step 4: Listen for incoming connections
    if (listen(m_socket, 20) < 0) {
        exit_with_error("Socket listen failed");
        return 1;
    }

    log("Listening on " + m_ip_address + ":" + std::to_string(m_port));

    // Step 5: Accept incoming connections
    acceptConnection();
    return 0;
}

void TcpServer::acceptConnection() {
    // Step 6: Accept new connection
    m_new_socket = accept(m_socket, (sockaddr*)&m_socketAddress, &m_socketAddress_len);
    if (m_new_socket < 0) {
        exit_with_error("Failed to accept connection");
    }

    log("Accepted connection!");

    // Step 7: Read client request
    char buffer[30720] = {0};
    long bytesReceived = read(m_new_socket, buffer, sizeof(buffer));
    if (bytesReceived < 0) {
        exit_with_error("Failed to read from client");
    }
    log("Client Request: " + std::string(buffer));

    // Step 8: Send HTTP response
    long bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());
    if (bytesSent != m_serverMessage.size()) {
        log("Error sending response to client");
    } else {
        log("Response sent to client");
    }

    // Close client socket
    close(m_new_socket);
}
    void TcpServer::closeServer(){
        close(m_socket);
        close(m_new_socket);
        exit(0);
    }
    
}