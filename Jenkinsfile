pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    /*stage('vcpkg bootstrap') {
      steps {
        sh './extern/vcpkg/bootstrap-vcpkg.sh'
      }
    }
    
    stage('CMake Configure') {
      steps {
        sh 'cmake -G Ninja -B build -S . -DCMAKE_TOOLCHAIN_FILE=extern/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE:STRING=Debug'
      }
    }*/

    stage('Make') {
      steps {
        sh 'cmake --build build/'
      }
    }

    stage('Install') {
      steps {
        sh 'cmake --install build/ --prefix install'
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