pipeline {
    
    agent any

    environment {
        // We need to create a separate folder for the CCS workspace
        // As it gets upset if we make it the same as our project directory
        CCS_WS_DIR = '/var/lib/jenkins/CCS_WS/'
        CCS_WS_exists = sh (
            script: 'if test -d /var/lib/jenkins/CCS_WS/; then echo "True"; else echo "False"; fi',
            returnStdout: true
        ).trim()
    }

    stages {
        stage('Warnings Report') {
            steps {
                warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
                step([$class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false, defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'CppLint', pattern: '**/checker_output/cpplint.txt']], unHealthy: ''])
            }
        }

        stage('Static Analysis') {
            steps {
                sh 'if [ -d "checker_output" ]; then rm -Rf checker_output; fi'
                sh 'mkdir checker_output'
                sh 'python cpplint.py --recursive src 2>&1 | tee ./checker_output/cpplint.txt'
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 -i"TIRTOS Build" -itest/ . 2> ./checker_output/cppcheck.xml'
            }
        }

        stage('Report Analysis') {
            steps {
                withCredentials([string(credentialsId: '36ba8ce1-8f3b-40cf-ac7d-1cff6b65e937', variable: 'personal_access_token')]) {
                    step([
                        $class: 'ViolationsToGitHubRecorder', 
                        config: [
                            gitHubUrl: 'https://api.github.com/', 
                            repositoryOwner: 'AKremor', 
                            repositoryName: 'msp_flight_software', 
                            pullRequestId: "1", 
                            createCommentWithAllSingleFileComments: false, 
                            createSingleFileComments: false, 
                            commentOnlyChangedContent: true, 
                            useOAuth2Token: false, 		
                            oAuth2Token: "", 		
                            useUsernamePassword: true, 		
                            username: 'MelbourneSpaceSteward', 		
                            password: "${personal_access_token}", 		
                            useUsernamePasswordCredentials: false, 		
                            usernamePasswordCredentialsId: '',
                            minSeverity: 'INFO',
                            keepOldComments: false,
                            violationConfigs: [
                                [ pattern: '.*/checker_output/.*\\.xml$', parser: 'CPPCHECK', reporter: 'CPPCHECK' ]
                            ]
                        ]
                    ]) 
                }
            }
        }


        stage('Build') { 
            steps {
                lock('my-resource-name') {
                    script {
                        if (Boolean.parseBoolean(CCS_WS_exists)) {
                            echo 'The workspace exists. Not importing new project.'
                        } else {
                            echo 'The workspace does not exist. Creating new project.'
                            sh 'mkdir -p ${CCS_WS_DIR}'
                            sh '/home/akremor/ti/ccsv7/eclipse/eclipse -noSplash -data "${CCS_WS_DIR}" -application com.ti.ccstudio.apps.projectImport -ccs.location $WORKSPACE'
                        }
                    }
                    sh '/home/akremor/ti/ccsv7/eclipse/eclipse -noSplash -data "${CCS_WS_DIR}" -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"'
                }
            }
        }
    }
}
