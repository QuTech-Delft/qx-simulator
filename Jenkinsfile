
pipeline {
    agent {
        dockerfile {
              filename 'Dockerfile.build'
        }
    }
    stages {
        stage('Build') {
            steps {
                sh 'pwd'
                sh 'mkdir build && cd build && cmake ../ && make'
            }
        }
        stage('Test') {
            steps {
                sh 'cd build/bin/qx-simulator/ && ./qx_simulator'

            }
        }

    }
}

@NonCPS
def echo_all(list) {
    list.each { item ->
        sh "echo Hello ${item}"
    }
}