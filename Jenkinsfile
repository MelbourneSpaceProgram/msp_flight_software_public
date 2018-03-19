pipeline {

    agent any

    environment {
        PR_ID = sh (
            script: 'if [ -z ${CHANGE_ID+x} ]; then echo "-1"; else echo "$CHANGE_ID"; fi',
            returnStdout: true
            ).trim()
    }
    
    stages {
       stage('Static Analysis') {
            agent {
                label 'master'
            }
            steps {
                sh 'if [ -d "checker_output" ]; then rm -Rf checker_output; fi'
                sh 'mkdir checker_output'
                sh 'python cpplint.py --recursive src 2>&1 | tee ./checker_output/cpplint.txt'
                // Check conditional branches for "#define A" only. This doesn't exist, which means we skip the poor-scaling of checking all possible
                // configurations, with the trade-off of weaker static analysis results.
                sh 'cppcheck -DA --enable=warning,performance,style,portability --inconclusive --xml --xml-version=2 -i"TIRTOS Build" -itest/ . 2> ./checker_output/cppcheck.xml'
                warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
                step([$class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false, defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', parserConfigurations: [[parserName: 'CppLint', pattern: '**/checker_output/cpplint.txt']], unHealthy: ''])
                sh 'cppcheck-htmlreport --source-encoding="iso8859-1" --title="MSP" --source-dir=. --report-dir=./checker_output/ --file=./checker_output/cppcheck.xml' 
                publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: './checker_output/', reportFiles: 'index.html', reportName: 'Static Analysis', reportTitles: ''])
                step([
                    $class: 'ViolationsToGitHubRecorder', 
                    config: [
                        gitHubUrl: 'https://api.github.com/', 
                        repositoryOwner: 'MelbourneSpaceProgram', 
                        repositoryName: 'flight_software', 
                        pullRequestId: "${PR_ID}", 
                        createCommentWithAllSingleFileComments: false, 
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
            agent {
                label 'AWS_Docker_Agent'
            }
            steps {
                sh '''
                    tar cvf CDH_software.tar.gz -C ${WORKSPACE} .
                    docker image ls
                    buildid=${BUILD_ID}
	            docker_name="ccs7build_$buildid"
                    docker run -td --name $docker_name ccs7_final_image_v1
                    docker exec -t $docker_name mkdir /tmp/code
                    docker cp ${WORKSPACE}/CDH_software.tar.gz $docker_name:/tmp/code
                    docker exec -t $docker_name tar -xf /tmp/code/CDH_software.tar.gz -C /tmp/code/
                    docker exec -t $docker_name /opt/ti/ccsv7/eclipse/eclipse -noSplash -data /opt/CDH_Software/workspace/ -application com.ti.ccstudio.apps.projectImport -ccs.location /tmp/code
                    docker exec -t $docker_name /opt/ti/ccsv7/eclipse/eclipse -noSplash -data /opt/CDH_Software/workspace/ -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"
                '''
            }
            post {
                always {
                    sh '''
		      buildid=${BUILD_ID}
	              docker_name="ccs7build_$buildid"
                      docker rm -f $docker_name
                    '''
                }
            }
        }
    }
}
