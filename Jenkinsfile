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
        PR_ID = sh (
            script: 'if [ -z ${CHANGE_ID+x} ]; then echo "-1"; else echo "$CHANGE_ID"; fi',
            returnStdout: true
            ).trim()
    }
    
    stages {
       stage('Static Analysis') {
            steps {
                sh 'if [ -d "checker_output" ]; then rm -Rf checker_output; fi'
                sh 'mkdir checker_output'
                sh 'python cpplint.py --recursive src 2>&1 | tee ./checker_output/cpplint.txt'
                sh 'cppcheck --enable=all --inconclusive --xml --xml-version=2 -i"TIRTOS Build" -itest/ . 2> ./checker_output/cppcheck.xml'
                warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
                step([$class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false, defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'CppLint', pattern: '**/checker_output/cpplint.txt']], unHealthy: ''])
                sh 'cppcheck-htmlreport --source-encoding="iso8859-1" --title="MSP" --source-dir=. --report-dir=./checker_output/ --file=./checker_output/cppcheck.xml' 
                sh 'echo PR_ID "${PR_ID}"'
                sh 'echo ghprbPullId "${ghprbPullId}"'
                publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: './checker_output/', reportFiles: 'index.html', reportName: 'Static Analysis', reportTitles: ''])
                step([
                    $class: 'ViolationsToGitHubRecorder', 
                    config: [
                        gitHubUrl: 'https://api.github.com/', 
                        repositoryOwner: 'MelbourneSpaceProgram', 
                        repositoryName: 'flight_software', 
                        pullRequestId: "${PR_ID}", 
                        createCommentWithAllSingleFileComments: true, 
                        createSingleFileComments: true, 
                        commentOnlyChangedContent: false, 
                        useOAuth2Token: false, 		
                        oAuth2Token: "", 		
                        useUsernamePassword: false, 		
                        username: '', 		
                        password: "", 		
                        useUsernamePasswordCredentials: true, 		
                        usernamePasswordCredentialsId: 'a2c805e2-79f1-4a00-9fa5-d8e144e50245',
                        minSeverity: 'INFO',
                        keepOldComments: false,
                        violationConfigs: [
                            [ pattern: '.*/checker_output/.*\\.xml$', parser: 'CPPCHECK', reporter: 'CPPCHECK' ]
                        ]
                    ]
                ])
            }
        }

        stage('Build') { 
            steps {
                lock('workspace') {
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
