pipeline {

    agent any

    environment {
        PR_ID = sh (
            script: 'if [ -z ${CHANGE_ID+x} ]; then echo "-1"; else echo "$CHANGE_ID"; fi',
            returnStdout: true
            ).trim()
	container_uuid = sh (
		script: "cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1",
		returnStdout: true,
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
                        credentialsId: 'a2c805e2-79f1-4a00-9fa5-d8e144e50245',
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
                    buildid=${BUILD_ID}
		    gitcommit=${GIT_COMMIT}
	            docker_name=$gitcommit"_"$buildid"_"${container_uuid}
                    docker run -td --name $docker_name ccs7_final_image_v1
                    docker cp ${WORKSPACE}/CDH_software.tar.gz $docker_name:/tmp/code
                    docker exec -t $docker_name tar -xf /tmp/code/CDH_software.tar.gz -C /tmp/code/
                    docker exec -t $docker_name /opt/ti/ccsv7/eclipse/eclipse -noSplash -data /opt/CDH_Software/workspace/ -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"
                    '''
    		    warnings canComputeNew: false, canResolveRelativePaths: false, canRunOnFailed: true, categoriesPattern: '', consoleParsers: [[parserName: 'Texas Instruments Code Composer Studio (C/C++)']], defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
    	    }
            post {
                always {
                    sh '''
		      buildid=${BUILD_ID}
                      gitcommit=${GIT_COMMIT}
                      docker_name=$gitcommit"_"$buildid"_"${container_uuid}
                      docker rm -f $docker_name
                    '''
                }
            }
        }

        stage('Unit Tests') { 
            agent {
                label 'flatsat'
            }
            steps {
                sh '''
                    tar cvf CDH_software.tar.gz -C ${WORKSPACE} .
                    buildid=${BUILD_ID}
                    gitcommit=${GIT_COMMIT}
                    docker_name=$gitcommit"_"$buildid"_"${container_uuid}"_unittest"
                    docker run -td --name $docker_name --privileged -v /dev/bus/usb:/dev/bus/usb jigglemein/ccs7_final_image_v3
                    docker cp ${WORKSPACE}/CDH_software.tar.gz $docker_name:/tmp/code
                    docker exec -t $docker_name tar -xf /tmp/code/CDH_software.tar.gz -C /tmp/code/
                    docker exec -t $docker_name /opt/ti/ccsv7/eclipse/eclipse -noSplash -data /opt/CDH_Software/workspace/ -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"
                    docker exec -t $docker_name /opt/ti/ccsv7/ccs_base/scripting/bin/dss.sh /tmp/code/targetConfigs/runner.js unittest
                    '''
            }
            post {
                always {
                    sh '''
                        buildid=${BUILD_ID}
                        gitcommit=${GIT_COMMIT}
                        docker_name=$gitcommit"_"$buildid"_"${container_uuid}"_unittest"
                        docker rm -f $docker_name
                    '''
                }
            }
        }

        stage('Day In The Life') { 
            agent {
                label 'flatsat'
            }
            when { branch "ditl/*" }
            steps {
                sh '''
                    tar cvf CDH_software.tar.gz -C ${WORKSPACE} .
                    buildid=${BUILD_ID}
                    gitcommit=${GIT_COMMIT}
                    docker_name=$gitcommit"_"$buildid"_"${container_uuid}"_ditl"
                    docker run -td --name $docker_name --privileged -v /dev/bus/usb:/dev/bus/usb jigglemein/ccs7_final_image_v3
                    docker cp ${WORKSPACE}/CDH_software.tar.gz $docker_name:/tmp/code
                    docker exec -t $docker_name tar -xf /tmp/code/CDH_software.tar.gz -C /tmp/code/
                    docker exec -t $docker_name /opt/ti/ccsv7/eclipse/eclipse -noSplash -data /opt/CDH_Software/workspace/ -application com.ti.ccstudio.apps.projectBuild -ccs.workspace -ccs.configuration "TIRTOS Build"
                    docker exec -t $docker_name /opt/ti/ccsv7/ccs_base/scripting/bin/dss.sh /tmp/code/targetConfigs/runner.js ditl
                    '''
            }
            post {
                always {
                    sh '''
                        buildid=${BUILD_ID}
                        gitcommit=${GIT_COMMIT}
                        docker_name=$gitcommit"_"$buildid"_"${container_uuid}"_ditl"
                        docker rm -f $docker_name
                    '''
                }
            }
        }
    }
}
