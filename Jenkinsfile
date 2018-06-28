
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
                sh 'cd src/ && ./configure && make && make install'
            }
        }
        stage('Test') {
            steps {
                sh for file in src/circuits/*.qc; do src/qx_simulator "$file"; done'

            }
        }

    }
}