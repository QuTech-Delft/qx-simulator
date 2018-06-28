
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
                sh 'cd src/ && ./qx_simulator circuits/epr.qc'

            }
        }

    }
}