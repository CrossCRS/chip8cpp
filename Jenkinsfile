pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('CMake Configure') {
      steps {
        sh 'cmake .'
      }
    }

    stage('Make') {
      steps {
        sh 'cmake --build .'
      }
    }

    stage('Install') {
      steps {
        sh 'cmake --install . --prefix install'
        archiveArtifacts artifacts: 'install/**/*', fingerprint: true
      }
    }

  }
  post {
    always {
      deleteDir()
    }
  }
}