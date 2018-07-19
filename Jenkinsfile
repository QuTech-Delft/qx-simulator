
pipeline {
   agent any
   environment {
       PERSIST_WORKSPACE = ""
   }
   stages {
        stage('Checkout submodules') {
            steps {
                script {
                    PERSIST_WORKSPACE = WORKSPACE
                }
                sh 'pwd'
                sh 'GIT_SSH_COMMAND="ssh -i /var/jenkins_home/.ssh/jenkins_libqasm_deploy_id_rsa" git submodule update --init -- src/parser/libqasm'
                sh 'cd src/parser/libqasm/ && git submodule update --init -- qasm_flex_bison/test/doctest'
            }
        }
        stage('Build') {
            agent {
                dockerfile {
                      filename 'Dockerfile.build'
                }
            }
            steps {
                dir(PERSIST_WORKSPACE) {
                    sh 'pwd'
                    sh 'mkdir build && cd build && cmake ../ && make'
                }
            }
        }
        stage('Test') {
            agent {
                dockerfile {
                      filename 'Dockerfile.build'
                }
            }
            steps {
                dir(PERSIST_WORKSPACE) {
                    sh 'for file in tests/circuits/*.qc; do build/bin/qx-simulator "$file"; done'
                }
            }
        }

    }
}
