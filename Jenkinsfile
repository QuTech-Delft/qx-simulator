
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
               sh 'for file in src/circuits/*.qc; do build/bin/qx-simulator/qx-simulator "$file"; done'

            }
        }

    }
}